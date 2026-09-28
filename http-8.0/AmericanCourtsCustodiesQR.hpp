#ifndef SLEELA_AMERICAN_COURTS_CUSTODIES_QR_HPP
#define SLEELA_AMERICAN_COURTS_CUSTODIES_QR_HPP

#include <cstddef>

namespace sleela::americas::qr {

/*
 * Square QR representation for the AmericanCourtsCustodies project.
 *
 * Payload: "SLEELA:AmericanCourtsCustodies"
 * Matrix: 37 x 37 modules, including a four-module quiet zone.
 * '#' = dark module; ' ' = light module.
 *
 * The matrix follows the square-module orientation used by QR Code
 * symbology. It is project data, not an official government identifier.
 */
inline constexpr std::size_t kMatrixSize = 37;
inline constexpr const char* kPayload = "SLEELA:AmericanCourtsCustodies";

inline constexpr const char* kMatrix[kMatrixSize] = {
    "                                     ",
    "                                     ",
    "                                     ",
    "                                     ",
    "    ####### ###           #######    ",
    "    #     #   ###   #  #  #     #    ",
    "    # ### #      # #####  # ### #    ",
    "    # ### # # # # ### #   # ### #    ",
    "    # ### # ###  ## #  #  # ### #    ",
    "    #     # #   #### ###  #     #    ",
    "    ####### # # # # # # # #######    ",
    "            ####  #  ###             ",
    "    #   # #### # ##### # #####  #    ",
    "      # #   # ###         ### #      ",
    "    ##  # #  # ##### ##  ##  #  #    ",
    "       # #  ###  # ###  #  ###  #    ",
    "      # ###   ####   # # #   ###     ",
    "     ### #    #  ## ###   ###        ",
    "       ## # ##  #   # #  ####   #    ",
    "    ##  #  # ##   #  ###  ## # ##    ",
    "      ## ## ########## # # # ####    ",
    "    ##  #     ###     #   #####      ",
    "      ### ##  ###### ## #    #  #    ",
    "       # # ###  ## ###  ##           ",
    "    ##    #### ###   # ######        ",
    "            ##  ### ### #   ## #     ",
    "    ####### ###     ## ## # # # #    ",
    "    #     #     # #  ####   ## #     ",
    "    # ### # ## # ### # ##########    ",
    "    # ### #   ##      #  #   ###     ",
    "    # ### #   ###### ######  # ##    ",
    "    #     #      ## ### ###  # ##    ",
    "    ####### ## #  # ##  #### # #     ",
    "                                     ",
    "                                     ",
    "                                     ",
    "                                     "
};

} // namespace sleela::americas::qr

#endif /* SLEELA_AMERICAN_COURTS_CUSTODIES_QR_HPP */
