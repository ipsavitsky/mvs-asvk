#include <algorithm>
#include <iostream>
#include <map>
#include <random>
#include <vector>

#include "tinyxml2.h"

int main() {
    tinyxml2::XMLDocument out_doc;
    std::default_random_engine rng(time(nullptr));
    auto numProgs = 4U;
    const std::vector<int> proc_loads{50, 70, 90, 100};
    auto sum = 0UL;
    for (auto i = 0U; i < numProgs; ++i) {
        std::uniform_int_distribution<> dst(0, proc_loads.size() - 1);
        auto *processes_element = out_doc.NewElement("processor");
        auto *load_element =
            processes_element->InsertNewChildElement("max_load");
        auto k = proc_loads[dst(rng)];
        load_element->SetText(k);
        sum += k;
        out_doc.InsertEndChild(processes_element);
    }
    std::cout << "average load/processor = " << sum / numProgs << std::endl;

    auto numProcs = numProgs * 8;
    sum = 0UL;
    const std::vector<int> prog_loads{5, 10, 15, 20};
    // const std::vector<int> prog_loads{5, 10, 15};
    for (auto i = 0U; i < numProcs; ++i) {
        std::uniform_int_distribution<> dst(0, prog_loads.size() - 1);
        auto *processes_element = out_doc.NewElement("process");
        auto *load_element = processes_element->InsertNewChildElement("load");
        auto pos1 = dst(rng);
        load_element->SetText(prog_loads[pos1]);
        sum += prog_loads[pos1];
        out_doc.InsertEndChild(processes_element);
    }

    std::cout << "generated load/processor = " << sum / numProgs << std::endl;

    // auto numCons = numProcs * 2;

    // std::vector<std::pair<int, int>> cons;
    std::multimap<int, int> mmap;

    for (auto i = 0U; i < numProcs; ++i) {
        std::uniform_int_distribution<> distribution(0, numProcs - 1);
        std::uniform_int_distribution<> dist(2, 3);
        for (auto j = mmap.count(i); j < dist(rng); ++j) {
            auto pos = distribution(rng);
            mmap.emplace(i, pos);
        }
    }

    const std::vector<int> cons_loads{10, 50, 70, 100};

    std::for_each(mmap.begin(), mmap.end(), [&](auto n) {
        auto *processes_element = out_doc.NewElement("connection");
        auto *load_element = processes_element->InsertNewChildElement("first");
        load_element->SetText(n.first);

        load_element = processes_element->InsertNewChildElement("second");
        load_element->SetText(n.second);

        std::uniform_int_distribution<> dst2(0, cons_loads.size() - 1);
        load_element = processes_element->InsertNewChildElement("load");
        load_element->SetText(cons_loads[dst2(rng)]);

        out_doc.InsertEndChild(processes_element);
    });

    out_doc.SaveFile("config.xml");
}
