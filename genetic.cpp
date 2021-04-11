#include "genetic.hpp"

#include <sstream>

auto Population::generate_from_xml(tinyxml2::XMLDocument &doc) -> void {
    tinyxml2::XMLElement *txtnode = doc.FirstChildElement("processor");
    std::stringstream convert_stream;
    int16_t proxy = 0;
    int16_t from = 0;
    int16_t to = 0;

    tinyxml2::XMLElement *maxload = nullptr;

    while (txtnode != nullptr) {
        maxload = txtnode->FirstChildElement("max_load");
        max_loads.emplace_back(atoi(maxload->GetText()));
        txtnode = txtnode->NextSiblingElement("processor");
    }
    txtnode = doc.FirstChildElement("process");
    while (txtnode != nullptr) {
        maxload = txtnode->FirstChildElement("load");
        progs.emplace_back(atoi(maxload->GetText()));
        txtnode = txtnode->NextSiblingElement("process");
    }
    txtnode = doc.FirstChildElement("connection");
    while (txtnode != nullptr) {
        maxload = txtnode->FirstChildElement("first");
        from = atoi(maxload->GetText());

        maxload = txtnode->FirstChildElement("second");
        to = atoi(maxload->GetText());

        maxload = txtnode->FirstChildElement("load");
        proxy = atoi(maxload->GetText());

        affils.emplace_back(std::make_pair(from, to), proxy);

        txtnode = txtnode->NextSiblingElement("connection");
    }
    txtnode = doc.FirstChildElement("preserved_high_position");
    preserved_high_positions = atoi(txtnode->GetText());

    txtnode = doc.FirstChildElement("preserved_low_position");
    preserved_low_positions = atoi(txtnode->GetText());

    txtnode = doc.FirstChildElement("T");
    T = atof(txtnode->GetText());

    txtnode = doc.FirstChildElement("crossing_volume");
    crossing_volume = atoi(txtnode->GetText());

    txtnode = doc.FirstChildElement("mutation_volume");
    mutation_volume = atoi(txtnode->GetText());
}

auto Population::print_vals() -> void {
    std::cout << "preserved_high_position: " << preserved_high_positions
              << std::endl
              << "preserved_low_position: " << preserved_low_positions
              << std::endl
              << "crossing_volume: " << crossing_volume << std::endl
              << "mutation_volume: " << mutation_volume << std::endl
              << "T: " << T << std::endl;
    std::cout << "max loads:" << std::endl;
    std::for_each(max_loads.begin(), max_loads.end(),
                  [](auto n) { std::cout << n << " "; });
    std::cout << std::endl << "progs:" << std::endl;
    std::for_each(progs.begin(), progs.end(),
                  [](auto n) { std::cout << n << " "; });
    std::cout << std::endl << "affils:" << std::endl;
    std::for_each(affils.begin(), affils.end(), [](auto n) {
        std::cout << n.first.first << "\t" << n.first.second << "\t" << n.second
                  << std::endl;
    });
}

auto Population::get_fittest() {
    sort_population();
    return population[0].first;
}

auto Population::run_generation() -> void {
    sort_population();
    selection();
    crossbreeding();
    mutation();
}

auto Population::calculate_target_fuction(const entity &ent) {
    size_t res = 0;
    for (auto x : affils) {
        if (ent.at(x.first.first) != ent.at(x.first.second)) {
            res += x.second;
        }
    }
    return res;
}

auto universal_crossbreeding(entity a, entity b) -> entity {
    entity res;
    static std::default_random_engine rng;
    std::uniform_int_distribution<> dst(0, 1);
    std::vector<affiliation> a_vec(a.begin(), a.end());
    std::vector<affiliation> b_vec(b.begin(), b.end());
    std::sort(a_vec.begin(), a_vec.end(), [](const auto &a, const auto &b) {
        if (a.first == b.first) {
            return a.second > b.second;
        }
        return a.first > b.first;
    });
    std::sort(b_vec.begin(), b_vec.end(), [](const auto &a, const auto &b) {
        if (a.first == b.first) {
            return a.second > b.second;
        }
        return a.first > b.first;
    });
    for (auto i = 0; i < a.size(); ++i) {
        res[i] = dst(rng) % 2 == 0 ? a[i] : b[i];
    }
    return res;
}

auto Population::crossbreeding() -> void {
    // auto bg = population.begin() + preserved_high_positions;
    static std::default_random_engine rng;
    std::uniform_int_distribution<> dst(0, population.size() - 1);
    for (auto i = 0; i < crossing_volume; ++i) {
        // TODO(me): make sure indeces are not equal
        entity new_ent = universal_crossbreeding(population[dst(rng)].first,
                                                 population[dst(rng)].first);
        population.emplace_back(new_ent, calculate_target_fuction(new_ent));
    }
}

auto switch_mutation(entity ent) -> entity {
    static std::default_random_engine rng;
    std::uniform_int_distribution<> dst(0, ent.size() - 1);
    std::swap(ent[dst(rng)], ent[dst(rng)]);
    return ent;
}

auto Population::mutation() -> void {
    static std::default_random_engine rng;
    std::uniform_int_distribution<> dst(0, population.size() - 1);
    for (auto i = 0; i < mutation_volume; ++i) {
        entity new_ent = switch_mutation(population[dst(rng)].first);
        population.emplace_back(new_ent, calculate_target_fuction(new_ent));
    }
}

auto Population::selection() -> void {
    std::vector<std::pair<entity, size_t>> new_population(population.size());
    for (int16_t i = 0; i < preserved_high_positions; ++i) {
        new_population.emplace_back(population[i]);
    }
    for (int16_t i = population.size();
         i > population.size() - preserved_low_positions; --i) {
        new_population.emplace_back(population[i]);
    }
    for (int16_t i = preserved_high_positions;
         i < preserved_high_positions +
                 (preserved_high_positions - preserved_low_positions) * T;
         ++i) {
        new_population.emplace_back(population[i]);
    }
    std::swap(population, new_population);
}

auto Population::sort_population() -> void {
    std::sort(population.begin(), population.end(),
              [](const auto &a, const auto &b) { return a.second > b.second; });
}