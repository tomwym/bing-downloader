
xmlXPathContextPtr xpathCtx = xmlXPathNewContext(home_xml->Doc());
if (xpathCtx == NULL) {
    std::cerr << "Failed to create XPath context." << std::endl;
    return 1;
}

const xmlChar* xpathExpr = (const xmlChar*)"//a";
xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression(xpathExpr, xpathCtx);

if (xpathObj == NULL || xpathObj->nodesetval == NULL || xpathObj->nodesetval->nodeNr == 0) {
    std::cerr << "XPath evaluation failed or no matching nodes found." << std::endl;
    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);
    xmlCleanupParser();
    return 1;
}
for (int i = 0; i < xpathObj->nodesetval->nodeNr; ++i) {
    xmlNodePtr node = xpathObj->nodesetval->nodeTab[i];
    std::cout << "Matched node: " << xmlNodeGetContent(node) << std::endl;
}

NULL_CHECK(root_element, return 0)


    // xmlNode* node = nullptr;
    // do {
    //     std::cout << "searching for year: " << year << ' ';
    //     node = FindFirstNodeWithTextValue(archive_node, std::to_string(year--));
    //     if (node)
    //         std::cout << "node name: " << (char*)node->name << '\n';
    // } while (node);

    // xmlNode* node_2024 = FindFirstNodeWithTextValue(archive_node, std::to_string(get_current_year()));
    // std::cout << "node name: " << (char*)node_2024->name << '\n';


        /**
     * Return the current year as an integer. For example 2024.
     */
    auto get_current_year = []() -> int {
        // https://stackoverflow.com/a/74758438
        using namespace std::chrono;
        return static_cast<int>(
            year_month_day{time_point_cast<days>(system_clock::now())}.year());
    };
    int year = get_current_year();


    /**
     * Process a local file into an xml node.
     * @param filename The filename without path or extension.
     *
     */
    auto GetLocalHTMLResource = [](std::string filename) -> std::unique_ptr<XMLContainer>
    {
        // If there is a leading '/', remove it.
        if (filename.front() == '/')
        {
            filename = filename.substr(1, filename.length()-1);
        }
        const std::filesystem::path local{ MAKE_PATH("./out") / MAKE_PATH(filename + ".html") };
        XMLContainerPtr xml {make_unique<XMLContainer>(local)};
        if (xml->Valid())
            return std::move(xml);
        return nullptr;
    };