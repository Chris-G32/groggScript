# generate_cpp_classes.py

class_template = '''#pragma once
#include "abstract_alphabet_node.hpp"

namespace GSAlphabet {{

class {class_name} : public AbstractAlphabetNode {{
public:
    {class_name}() {{}}
    virtual void accept(AbstractAlphabetNodeVisitor* visitor) override;
}};

inline void {class_name}::accept(AbstractAlphabetNodeVisitor* visitor) {{
    visitor->visit{class_name}(this);
}}

}} // namespace GSAlphabet
'''

def generate_class_file(class_name):
    filename = f"{class_name.lower()}.hpp"
    with open(filename, 'w') as f:
        f.write(class_template.format(class_name=class_name))
    print(f"Generated {filename}")

visit_method_template = '    virtual void visit{class_name}({class_name}* node) = 0;'

def generate_visitor(class_names):
    visit_methods = "\n".join(
        visit_method_template.format(class_name=name) for name in class_names
    )
    return visit_methods


def main():
    class_names = [
        "Symbol",
        "PrimitiveValue",
        "Operator",
    ]
    
    for name in class_names:
        generate_class_file(name)

    content = generate_visitor(class_names)
    with open("abstract_alphabet_node_visitor.hpp", "w") as f:
        f.write(content)

    print("Generated abstract_alphabet_node_visitor.hpp")


if __name__ == "__main__":
    main()
