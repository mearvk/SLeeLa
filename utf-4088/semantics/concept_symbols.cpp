#include "concept_symbols.hpp"

#include <array>
#include <string>
#include <string_view>
#include <vector>

namespace utf4088::semantics {
namespace {

struct ConceptSeed {
    std::uint32_t id;
    std::string_view key;
    std::string_view label;
    std::string_view domain;
    std::string_view definition;
    std::string_view related_keys;
    std::array<double, 4> anchor_4d;
};

constexpr std::array<ConceptSeed, 44> kConceptSeeds{{
    {1U, "wealth", "Wealth", "economy", "Stock of economic assets and resources, net of liabilities where that accounting context applies; distinct from income and money.", "money,income,property,resources,value,trade", {0.100000, 0.150000, 0.200000, 0.250000}},
    {2U, "money", "Money", "economy", "A medium of exchange, unit of account, or store of value within a specified social or institutional system; not identical to wealth.", "wealth,trade,debt,value", {0.112000, 0.164000, 0.206000, 0.259000}},
    {3U, "income", "Income", "economy", "A flow of earnings or receipts over a time interval; distinct from accumulated wealth or a balance-sheet stock.", "wealth,trade,resources", {0.124000, 0.178000, 0.212000, 0.268000}},
    {4U, "property", "Property", "economy", "An asset or resource together with recognized ownership, use, or control rights.", "wealth,resources,trade,wood", {0.136000, 0.152000, 0.218000, 0.277000}},
    {5U, "trade", "Trade", "economy", "An exchange of goods, services, or rights between parties under a specified transaction context.", "money,wealth,quality,region", {0.103000, 0.166000, 0.224000, 0.286000}},
    {6U, "debt", "Debt", "economy", "An obligation to repay money, goods, or another specified value; distinct from an asset or from wealth itself.", "money,wealth,property", {0.115000, 0.180000, 0.230000, 0.253000}},
    {7U, "resources", "Resources", "economy", "Useful assets, materials, capabilities, or supplies available for a purpose; the resource type should be explicit.", "wealth,property,wood,water", {0.127000, 0.154000, 0.236000, 0.262000}},
    {8U, "wood", "Wood", "material_nature", "The structural material derived from tree tissues; distinct from a living tree, a forest ecosystem, or processed timber.", "tree,forest,timber,plant,resources", {0.800000, 0.150000, 0.280000, 0.300000}},
    {9U, "tree", "Tree", "material_nature", "A perennial woody plant with a trunk or comparable main stem and branching crown; biological details depend on taxon.", "wood,forest,plant,environment", {0.812000, 0.164000, 0.286000, 0.309000}},
    {10U, "forest", "Forest", "material_nature", "An ecosystem or land area dominated by trees and associated organisms and processes; not a synonym for wood.", "tree,wood,plant,environment,asia", {0.824000, 0.178000, 0.292000, 0.318000}},
    {11U, "timber", "Timber", "material_nature", "Wood prepared or designated for construction, woodworking, or other material use; usage may vary by region.", "wood,tree,property,quality", {0.836000, 0.152000, 0.298000, 0.327000}},
    {12U, "plant", "Plant", "material_nature", "A plant organism or member of the plant kingdom, interpreted at the taxonomic scope specified by context.", "tree,forest,wood,life,environment", {0.803000, 0.166000, 0.304000, 0.336000}},
    {13U, "stone", "Stone", "material_nature", "A naturally occurring piece of rock or mineral material, distinguished from a geological formation.", "wood,water,resources,quality", {0.815000, 0.180000, 0.310000, 0.303000}},
    {14U, "water", "Water", "material_nature", "The substance H2O or a specified body or supply of it; distinguish substance, resource, and geographic-feature senses.", "resources,life,environment,asia", {0.827000, 0.154000, 0.316000, 0.312000}},
    {15U, "sex", "Sex (context-dependent)", "human_experience", "An intentionally broad, ambiguous head concept. Resolve to biological sex, sexual activity, or sexuality only when context supports that distinction.", "biological_sex,sexual_activity,sexuality,consent", {0.150000, 0.800000, 0.680000, 0.740000}},
    {16U, "biological_sex", "Biological sex", "human_experience", "Biological sex characteristics or classification in a stated medical, biological, or demographic context; not a synonym for gender identity.", "sex,reproduction,health", {0.162000, 0.814000, 0.686000, 0.749000}},
    {17U, "sexual_activity", "Sexual activity", "human_experience", "Sexual behavior described neutrally for the relevant health, legal, or social context; consent must be represented separately.", "sex,sexuality,consent,pleasure,intimacy", {0.174000, 0.828000, 0.692000, 0.758000}},
    {18U, "sexuality", "Sexuality", "human_experience", "The broader domain of sexual identity, orientation, feelings, and expression as defined by the application; not reducible to one act or biological trait.", "sex,sexual_activity,desire,intimacy", {0.186000, 0.802000, 0.698000, 0.767000}},
    {19U, "pleasure", "Pleasure", "human_experience", "A positive subjective hedonic experience; distinct from long-term wellbeing, satisfaction of a goal, and moral approval.", "satisfaction,wellbeing,desire,pain", {0.153000, 0.816000, 0.704000, 0.776000}},
    {20U, "desire", "Desire", "human_experience", "A motivational state directed toward an object, outcome, or experience; specify its object and context when useful.", "pleasure,sexuality,wealth,relationship", {0.165000, 0.830000, 0.710000, 0.743000}},
    {21U, "intimacy", "Intimacy", "human_experience", "Closeness, trust, or private connection between people, with emotional, social, and sexual dimensions distinguished by context.", "relationship,sexual_activity,sexuality,consent", {0.177000, 0.804000, 0.716000, 0.752000}},
    {22U, "consent", "Consent", "human_experience", "Voluntary, informed, specific, and revocable agreement within a defined context; applicable legal requirements depend on jurisdiction.", "sexual_activity,relationship,ethics", {0.189000, 0.818000, 0.722000, 0.761000}},
    {23U, "reproduction", "Reproduction", "human_experience", "The biological process by which organisms produce offspring; separate from sexual activity, fertility status, and social parenthood.", "biological_sex,life,health", {0.156000, 0.832000, 0.728000, 0.770000}},
    {24U, "wellbeing", "Wellbeing", "human_experience", "A multidimensional state of health and quality of life; not identical to momentary pleasure or a single metric.", "pleasure,health,satisfaction,quality,pain", {0.168000, 0.806000, 0.734000, 0.779000}},
    {25U, "satisfaction", "Satisfaction", "human_experience", "A judgment or feeling that a need, expectation, or goal has been met; distinguish from pleasure and objective quality.", "pleasure,wellbeing,quality,desire", {0.180000, 0.820000, 0.740000, 0.746000}},
    {26U, "pain", "Pain", "human_experience", "An unpleasant sensory or emotional experience associated with or resembling actual or potential tissue damage; context may be physical or psychological.", "pleasure,health,wellbeing", {0.192000, 0.834000, 0.746000, 0.755000}},
    {27U, "health", "Health", "human_experience", "Physical, mental, and social functioning under the chosen health framework; not merely the absence of disease.", "wellbeing,pain,reproduction,quality", {0.159000, 0.808000, 0.752000, 0.764000}},
    {28U, "relationship", "Relationship", "human_experience", "A connection or association between people, entities, or concepts; specify the relationship type when it affects interpretation.", "intimacy,consent,desire,trade", {0.171000, 0.822000, 0.758000, 0.773000}},
    {29U, "quality", "Quality", "evaluation", "A property or degree assessed against an explicit criterion, purpose, or standard; not an absolute universal score.", "standard,excellence,defect,value,merit", {0.500000, 0.500000, 0.430000, 0.460000}},
    {30U, "quantity", "Quantity", "evaluation", "An amount or count measured in a declared unit or counting convention; distinct from quality.", "quality,value,resources", {0.512000, 0.514000, 0.436000, 0.469000}},
    {31U, "value", "Value", "evaluation", "Worth or significance under a specified economic, practical, ethical, or personal framework; record the value type and viewpoint.", "wealth,money,quality,merit,trade", {0.524000, 0.528000, 0.442000, 0.478000}},
    {32U, "merit", "Merit", "evaluation", "A feature considered deserving of recognition under stated criteria; not a universal ranking of people or cultures.", "quality,value,excellence,standard", {0.536000, 0.502000, 0.448000, 0.487000}},
    {33U, "standard", "Standard", "evaluation", "A declared rule, criterion, or reference level against which an object, quality, or process may be assessed.", "quality,excellence,defect,value", {0.503000, 0.516000, 0.454000, 0.496000}},
    {34U, "defect", "Defect", "evaluation", "A departure from a stated requirement or intended function; classification depends on the declared standard and use.", "quality,standard,excellence", {0.515000, 0.530000, 0.460000, 0.463000}},
    {35U, "excellence", "Excellence", "evaluation", "A high level of performance or quality against explicit criteria; the criteria and supporting evidence should be recorded.", "quality,merit,standard", {0.527000, 0.504000, 0.466000, 0.472000}},
    {36U, "asia", "Asia", "geography_culture", "A broad geographic region or continent under a chosen geographic convention; do not infer one culture, language, or identity from the place label.", "continent,region,country,geography,culture,language", {0.820000, 0.800000, 0.300000, 0.340000}},
    {37U, "continent", "Continent", "geography_culture", "A large landmass or recognized continental region under a named geographic convention; boundaries may vary by convention.", "asia,geography,region", {0.832000, 0.814000, 0.306000, 0.349000}},
    {38U, "region", "Region", "geography_culture", "A geographic or functional area defined by explicit boundaries or criteria; the label alone does not define political status or culture.", "asia,continent,country,geography,forest", {0.844000, 0.828000, 0.312000, 0.358000}},
    {39U, "country", "Country", "geography_culture", "A country or state under a declared political or geographic model; country, nation, culture, and language are not interchangeable.", "region,asia,culture,language,trade", {0.856000, 0.802000, 0.318000, 0.367000}},
    {40U, "geography", "Geography", "geography_culture", "The study or representation of places, spatial relations, environments, and human activity; distinct from any single place name.", "asia,continent,region,environment", {0.823000, 0.816000, 0.324000, 0.376000}},
    {41U, "culture", "Culture", "geography_culture", "Shared and changing practices, meanings, institutions, and artifacts in a group or setting; avoid assuming a country has only one culture.", "asia,region,country,language", {0.835000, 0.830000, 0.330000, 0.343000}},
    {42U, "language", "Language", "geography_culture", "A system of communication with linguistic structure and community use; not equivalent to ethnicity, nationality, or culture.", "culture,region,asia", {0.847000, 0.804000, 0.336000, 0.352000}},
    {43U, "environment", "Environment", "material_nature", "The surrounding physical, biological, and social conditions relevant to an organism or system; define the boundary and scale.", "forest,tree,plant,water,geography", {0.839000, 0.168000, 0.322000, 0.321000}},
    {44U, "life", "Life", "material_nature", "The state or processes associated with living organisms under the selected domain definition; no universal operational definition is assumed.", "plant,tree,reproduction,health,environment", {0.806000, 0.182000, 0.328000, 0.330000}}
}};

} // namespace

const std::vector<ConceptSymbol>& concept_symbol_registry() {
    static const std::vector<ConceptSymbol> registry = [] {
        std::vector<ConceptSymbol> result;
        result.reserve(kConceptSeeds.size());
        for (const auto& seed : kConceptSeeds) {
            std::string glyph_seed = "UTF4088-CONCEPT-V1:";
            glyph_seed += seed.key;
            const auto shape = seed_glyph(glyph_seed);
            result.push_back({
                seed.id, seed.key, seed.label, seed.domain, seed.definition,
                seed.related_keys, seed.anchor_4d, std::move(glyph_seed),
                shape, analyze_glyph(shape)
            });
        }
        return result;
    }();
    return registry;
}

const ConceptSymbol* find_concept_symbol(std::string_view key) {
    const auto& registry = concept_symbol_registry();
    for (const auto& symbol : registry) {
        if (symbol.key == key) return &symbol;
    }
    return nullptr;
}

} // namespace utf4088::semantics
