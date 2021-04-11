#include "tinyxml2.h"
#include "genetic.hpp"
#include <iostream>

int main() {
    tinyxml2::XMLDocument doc;
    doc.LoadFile("killme.xml");
    Population popl;
    popl.generate_from_xml(doc);
    popl.print_vals();
}