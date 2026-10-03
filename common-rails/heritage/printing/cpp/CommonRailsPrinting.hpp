#pragma once
#include <ostream>
#include <string>
#include <algorithm>
namespace commonrails { namespace printing {
struct PrintLayout { static constexpr int width=80, objectIdWidth=10, currentWidth=39, side=21, cells=441; };
enum class PrintState { START, WORKING, PROGRESS, COMPLETE, WARN, ERROR };
struct PrintGlyphs { static constexpr const char* full="█"; static constexpr const char* empty="░"; };
struct PrintField { static std::string pad(const std::string&s,int w); };
struct PrintLine { static void write(std::ostream&o,const std::string&s,int w=PrintLayout::width); };
struct PrintProgress { static int clamp(int p); static int cells(int p); static void write(std::ostream&o,int p); };
struct PrintComponent { static void write(std::ostream&o,const std::string&name,unsigned long id,unsigned long date,const std::string&message); };
struct PrintFormatter { static void status(std::ostream&o,PrintState s,const std::string&message); };
struct PrintRenderer { static void square(std::ostream&o,int filled); };
struct PrintWriter { explicit PrintWriter(std::ostream&o):out(o){} std::ostream&out; };
}}
