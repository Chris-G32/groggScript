#ifndef GROGGSCRIPT_SOURCE_LOCATION_HPP
#define GROGGSCRIPT_SOURCE_LOCATION_HPP
struct SourceLocation {
    unsigned long line;
    unsigned long column;
};

struct SourceLocationSpan {
    SourceLocation start;
    SourceLocation end;
};
#endif  // GROGGSCRIPT_SOURCE_LOCATION_HPP
