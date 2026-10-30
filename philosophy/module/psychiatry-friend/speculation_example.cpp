#include "speculation_invite.hpp"
#include <iostream>
#include <string>

/*
 * Interactive demo. Shows the invitation, lets a professional type
 * speculations (interactive mode), or runs a careful seeded set
 * non-interactively, then prints the scientific report.
 *
 * Interactive input per speculation (blank author ends input):
 *   author | role | scope | aspect(0-4) | confidence(0-2) | text | falsifiability
 * each on its own line.
 */
using namespace sleela::philosophy;

static void seed_examples(SpeculationInvite &inv) {
    Speculation a;
    a.id = 1u;
    a.author = "Dr. A. Reviewer";
    a.role = "adult psychiatrist (self-attested, unverified)";
    a.scope = "the consent and provenance rules in this module";
    a.aspect = SL_ASPECT_LIMITATION;
    a.confidence = SL_CONF_WELL_REASONED;
    a.text = "The software's reach should stop at organizing a person's own "
             "supplied notes; it should not infer internal states it was never "
             "told, because that would overstep what explicit input supports.";
    a.falsifiability = "Shown wrong if a controlled review finds the software "
             "reliably reporting only what was entered, with no inferred states.";
    inv.submitChecked(a);

    Speculation b;
    b.id = 2u;
    b.author = "Dr. B. Clinician";
    b.role = "adult psychiatry fellow (self-attested, unverified)";
    b.scope = "the revisit/related-event linking behavior";
    b.aspect = SL_ASPECT_SAFETY;
    b.confidence = SL_CONF_PLAUSIBLE;
    b.text = "Treating a repeated or replayed thought as independent "
             "corroboration could reinforce a fixed belief; the programming "
             "should keep revisits labelled as the same source.";
    b.falsifiability = "Shown wrong if revisit links are demonstrably never "
             "counted as new evidence anywhere in the pipeline.";
    inv.submitChecked(b);

    Speculation c;
    c.id = 3u;
    c.author = "Dr. C. Researcher";
    c.role = "academic psychiatrist (self-attested, unverified)";
    c.scope = "a suggested study";
    c.aspect = SL_ASPECT_RESEARCH_DIRECTION;
    c.confidence = SL_CONF_TENTATIVE;
    c.text = "A useful study would measure whether structured, consent-gated "
             "note-taking helps people prepare better questions for a human "
             "clinician, without the tool implying any assessment.";
    c.falsifiability = "Shown wrong if a trial finds no difference in question "
             "quality versus unstructured notes.";
    inv.submitChecked(c);
}

static int read_int(const std::string &line, int lo, int hi, int fallback) {
    try {
        int v = std::stoi(line);
        if (v < lo || v > hi) return fallback;
        return v;
    } catch (...) { return fallback; }
}

int main(int argc, char **argv) {
    SpeculationInvite inv(64u);
    std::cout << SpeculationInvite::invitation() << "\n\n";

    bool interactive = (argc >= 2 && std::string(argv[1]) == "--interactive");
    if (interactive) {
        std::cout << "Enter speculations. Blank author line ends input.\n"
                     "Order: author / role / scope / aspect(0-4) / confidence(0-2)"
                     " / text / falsifiability\n\n";
        std::uint64_t id = 1u;
        std::string author;
        while (true) {
            std::cout << "author (blank to finish): ";
            if (!std::getline(std::cin, author) || author.empty()) break;
            Speculation s;
            s.id = id++;
            s.author = author;
            std::string line;
            std::cout << "role (self-reported): ";      std::getline(std::cin, s.role);
            std::cout << "scope: ";                       std::getline(std::cin, s.scope);
            std::cout << "aspect 0=cap 1=limit 2=safety 3=ethics 4=research: ";
            std::getline(std::cin, line);
            s.aspect = static_cast<sl_spec_aspect>(read_int(line, 0, 4, 1));
            std::cout << "confidence 0=tentative 1=plausible 2=well-reasoned: ";
            std::getline(std::cin, line);
            s.confidence = static_cast<sl_spec_confidence>(read_int(line, 0, 2, 0));
            std::cout << "speculation text: ";           std::getline(std::cin, s.text);
            std::cout << "how it could be shown wrong: ";std::getline(std::cin, s.falsifiability);
            if (inv.submitChecked(s)) std::cout << "  recorded.\n\n";
            else std::cout << "  not recorded (needs author, text, and a "
                               "falsifiability note).\n\n";
        }
    } else {
        seed_examples(inv);
    }

    std::cout << "\n";
    inv.writeReport(std::cout);
    return 0;
}
