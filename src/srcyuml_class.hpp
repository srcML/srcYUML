/**
 * @file srcyuml_class.hpp
 *
 * @copyright Copyright (C) 2016 srcML, LLC. (www.srcML.org)
 *
 * This file is part of srcYUML.
 *
 * srcYUML is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * srcYUML is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with srcYUML.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef INCLUDED_SRCYUML_CLASS_HPP
#define INCLUDED_SRCYUML_CLASS_HPP

#include <ClassPolicySingleEvent.hpp>

class srcyuml_class {

private:

    enum class_type { NONE, INTERFACE, ABSTRACT, DATATYPE };

    const ClassPolicy::ClassData * data;

    bool has_field;
    bool has_constructor;
    bool has_destructor;
    bool has_method;

    class_type type;

public:
    srcyuml_class(const ClassPolicy::ClassData * data)
        : data(data),
        has_field(false),
        has_constructor(false),
        has_destructor(false),
        has_method(false),
        type(NONE) {}

    // ~srcyuml_class() { delete data; }

    void analyze_data() {

        has_field = data->fields[ClassPolicy::PUBLIC].size() || data->fields[ClassPolicy::PRIVATE].size() || data->fields[ClassPolicy::PROTECTED].size();
        has_constructor = data->constructors[ClassPolicy::PUBLIC].size() || data->constructors[ClassPolicy::PRIVATE].size() || data->constructors[ClassPolicy::PROTECTED].size();
        has_destructor = data->hasDestructor;
        has_method = data->methods[ClassPolicy::PUBLIC].size() || data->methods[ClassPolicy::PRIVATE].size() || data->methods[ClassPolicy::PROTECTED].size();

        bool only_public_methods = data->methods[ClassPolicy::PUBLIC].size() && data->methods[ClassPolicy::PRIVATE].empty() && data->methods[ClassPolicy::PROTECTED].empty();

       // if((overloaded_assignment && copy_constructor && default_constructor) || (!has_constructor && !overloaded_assignment && assignment_operator.function_name == "")) {
        if(!has_constructor && !has_method) {

            type = DATATYPE;

        } else if(    !has_constructor
            && !has_field
            && !has_destructor
            && only_public_methods
            // && (assignment_operator.function_name == "" || (assignment_operator.pure_virtual && overloaded_assignment))
            ) {

            bool is_interface = true;
            for(FunctionSignaturePolicy::FunctionSignatureData * function : data->methods[ClassPolicy::PUBLIC]) {

                if(!function->isPureVirtual) {
                    is_interface = false;
                }

            }

            if(is_interface)
                type = INTERFACE;

        }


    }


    friend std::ostream & operator<<(std::ostream & out, const srcyuml_class & aclass) {

        out << '[';

        if(aclass.type == DATATYPE)
            out << "«datatype»;";
        else if(aclass.type == INTERFACE)
            out << "«interface»;";

        // not sure if should be gulliments or {}
        else if(aclass.type == ABSTRACT)
            out << "«abstract»;";

        out << *aclass.data->name;

        if(aclass.has_field || aclass.has_method)
            out << '|';

        for(DeclTypePolicy::DeclTypeData * field : aclass.data->fields[ClassPolicy::PUBLIC]) {
            out << "+ " << *field << ';';
        }

        for(DeclTypePolicy::DeclTypeData * field : aclass.data->fields[ClassPolicy::PRIVATE]) {
            out << "- " << *field << ';';
        }

        for(DeclTypePolicy::DeclTypeData * field : aclass.data->fields[ClassPolicy::PROTECTED]) {
            out << "# " << *field << ';';
        }

        if(aclass.has_method)
            out << '|';

        for(FunctionSignaturePolicy::FunctionSignatureData * function : aclass.data->methods[ClassPolicy::PUBLIC]) {
            out << "+ " << *function << ';';
        }

        for(FunctionSignaturePolicy::FunctionSignatureData * function : aclass.data->methods[ClassPolicy::PRIVATE]) {
            out << "- " << *function << ';';
        }

        for(FunctionSignaturePolicy::FunctionSignatureData * function : aclass.data->methods[ClassPolicy::PROTECTED]) {
            out << "# " << *function << ';';
        }

        out << "]\n";

        return out;

    }

};

 #endif