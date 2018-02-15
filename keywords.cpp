/*===========================================================================
    CLIPSEditor, editor for CLIPS (C Language Integrated Production System)
    Copyright (C) 2012-2018 Artem G. Novikov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>
===========================================================================*/

#include "keywords.h"

Keywords::Keywords()
{
    QString type = "Agenda";
    keywords.insert(type, "agenda");
    keywords.insert(type, "clear-focus-stack");
    keywords.insert(type, "focus");
    keywords.insert(type, "get-salience-evaluation");
    keywords.insert(type, "get-strategy");
    keywords.insert(type, "halt");
    keywords.insert(type, "list-focus-stack");
    keywords.insert(type, "refresh-agenda");
    keywords.insert(type, "run");
    keywords.insert(type, "set-salience-evaluation");
    keywords.insert(type, "set-strategy");
    keywords.insert(type, "get-focus");
    keywords.insert(type, "get-focus-stack");
    keywords.insert(type, "pop-focus");
   
    type = "Constraints";
    keywords.insert(type, "SYMBOL");
    keywords.insert(type, "STRING");
    keywords.insert(type, "LEXEME");
    keywords.insert(type, "INTEGER");
    keywords.insert(type, "FLOAT");
    keywords.insert(type, "NUMBER");
    keywords.insert(type, "INSTANCE-NAME");
    keywords.insert(type, "INSTANCE-ADDRESS");
    keywords.insert(type, "INSTANCE");
    keywords.insert(type, "EXTERNAL-ADDRESS");
    keywords.insert(type, "FACT-ADDRESS");
    keywords.insert(type, "allowed-symbols");
    keywords.insert(type, "allowed-strings");
    keywords.insert(type, "allowed-lexemes");
    keywords.insert(type, "allowed-integers");
    keywords.insert(type, "allowed-floats");
    keywords.insert(type, "allowed-numbers");
    keywords.insert(type, "allowed-instance-names");
    keywords.insert(type, "allowed-classes");
    keywords.insert(type, "allowed-values");
    keywords.insert(type, "range");
    keywords.insert(type, "cardinality");
    keywords.insert(type, "type");
    
    type = "Constructs";
    keywords.insert(type, "defclass");
    keywords.insert(type, "deffacts");
    keywords.insert(type, "deffunction");
    keywords.insert(type, "defgeneric");
    keywords.insert(type, "defmethod");
    keywords.insert(type, "defglobal");
    keywords.insert(type, "definstances");
    keywords.insert(type, "defmessage-handler");
    keywords.insert(type, "defmodule");
    keywords.insert(type, "defrule");
    keywords.insert(type, "deftemplate");
    
    type = "COOL Classes";
    keywords.insert(type, "role");
    keywords.insert(type, "concrete");
    keywords.insert(type, "abstract");
    keywords.insert(type, "pattern-match");
    keywords.insert(type, "reactive");
    keywords.insert(type, "non-reactiv");
    keywords.insert(type, "storage");
    keywords.insert(type, "local");
    keywords.insert(type, "shared");
    keywords.insert(type, "access");
    keywords.insert(type, "read-write");
    keywords.insert(type, "read-only");
    keywords.insert(type, "initialize-only");
    keywords.insert(type, "propagation");
    keywords.insert(type, "inherit");
    keywords.insert(type, "no-inherit");
    keywords.insert(type, "source");
    keywords.insert(type, "exclusive");
    keywords.insert(type, "composite");
    keywords.insert(type, "pattern-match");
    keywords.insert(type, "reactive");
    keywords.insert(type, "non-reactive");
    keywords.insert(type, "visibility");
    keywords.insert(type, "private");
    keywords.insert(type, "public");
    keywords.insert(type, "create-accessor");
    keywords.insert(type, "read");
    keywords.insert(type, "write");
    keywords.insert(type, "read-write");
    keywords.insert(type, "override-message");
    keywords.insert(type, "is-a");
    keywords.insert(type, "object");
    keywords.insert(type, "name");
    keywords.insert(type, "browse-classes");
    keywords.insert(type, "describe-class");
    keywords.insert(type, "list-defclasses");
    keywords.insert(type, "ppdefclass");
    keywords.insert(type, "undefclass");
    keywords.insert(type, "class-abstractp");
    keywords.insert(type, "class-existp");
    keywords.insert(type, "class-reactivep");
    keywords.insert(type, "class-slots");
    keywords.insert(type, "class-subclasses");
    keywords.insert(type, "class-superclasses");
    keywords.insert(type, "defclass-module");
    keywords.insert(type, "get-class-defaults-mode");
    keywords.insert(type, "get-defclass-list");
    keywords.insert(type, "get-defmessage-handler-list");
    keywords.insert(type, "message-handler-existp");
    keywords.insert(type, "set-class-defaults-mode");
    keywords.insert(type, "slot-allowed-classes");
    keywords.insert(type, "slot-allowed-values");
    keywords.insert(type, "slot-cardinality");
    keywords.insert(type, "slot-default-value");
    keywords.insert(type, "slot-direct-accessp");
    keywords.insert(type, "slot-existp");
    keywords.insert(type, "slot-facets");
    keywords.insert(type, "slot-initablep");
    keywords.insert(type, "slot-publicp");
    keywords.insert(type, "slot-range");
    keywords.insert(type, "slot-sources");
    keywords.insert(type, "slot-types");
    keywords.insert(type, "slot-writablep");
    keywords.insert(type, "subclassp");
    keywords.insert(type, "superclassp");

    type = "COOL Handlers";
    keywords.insert(type, "call-next-handler");
    keywords.insert(type, "next-handlerp");
    keywords.insert(type, "override-next-handler");
    keywords.insert(type, "list-defmessage-handlers");
    keywords.insert(type, "ppdefmessage-handler");
    keywords.insert(type, "preview-send");
    keywords.insert(type, "undefmessage-handler");
    keywords.insert(type, "send");
    keywords.insert(type, "put");
    keywords.insert(type, "get");
    keywords.insert(type, "primary");
    keywords.insert(type, "around");
    keywords.insert(type, "before");
    keywords.insert(type, "after");
    keywords.insert(type, "message-handler");
    
    type = "COOL Instancess";
    keywords.insert(type, "list-definstances");
    keywords.insert(type, "ppdefinstances");
    keywords.insert(type, "undefinstances");
    keywords.insert(type, "definstances-module");
    keywords.insert(type, "get-definstances-list");
    keywords.insert(type, "instances");
    keywords.insert(type, "load-instances");
    keywords.insert(type, "ppinstance");
    keywords.insert(type, "restore-instances");
    keywords.insert(type, "save-instances");
    keywords.insert(type, "bload-instances");
    keywords.insert(type, "bsave-instances");
    keywords.insert(type, "any-instancep");
    keywords.insert(type, "find-instance");
    keywords.insert(type, "find-all-instances");
    keywords.insert(type, "do-for-instance");
    keywords.insert(type, "do-for-all-instances");
    keywords.insert(type, "delayed-do-for-all-instances");
    keywords.insert(type, "class");
    keywords.insert(type, "delete-instance");
    keywords.insert(type, "duplicate-instance");
    keywords.insert(type, "init-slots");
    keywords.insert(type, "instance-address");
    keywords.insert(type, "instance-key");
    keywords.insert(type, "instance-key-to-symbol");
    keywords.insert(type, "instance-name");
    keywords.insert(type, "instance-name-to-symbol");
    keywords.insert(type, "instance-namep");
    keywords.insert(type, "symbol-to-instance-name");
    keywords.insert(type, "symbol-to-instance-key");
    keywords.insert(type, "make-instance");
    keywords.insert(type, "initialize-instance");
    keywords.insert(type, "unmake-instance");
    keywords.insert(type, "active-duplicate-instance");
    keywords.insert(type, "active-initialize-instance");
    keywords.insert(type, "active-make-instance");
    keywords.insert(type, "active-message-duplicate-instance");
    keywords.insert(type, "active-message-modify-instance");
    keywords.insert(type, "active-modify-instance");
    keywords.insert(type, "message-duplicate-instance");
    keywords.insert(type, "message-modify-instance");
    keywords.insert(type, "modify-instance");
    keywords.insert(type, "object-pattern-match-delay");
    keywords.insert(type, "direct-slot-delete$");
    keywords.insert(type, "direct-slot-insert$");
    keywords.insert(type, "direct-slot-replace$");
    keywords.insert(type, "dynamic-get");
    keywords.insert(type, "dynamic-put");
    keywords.insert(type, "slot-delete$");
    keywords.insert(type, "slot-insert$");
    keywords.insert(type, "slot-replace$");
    keywords.insert(type, "instance-addressp");
    keywords.insert(type, "instance-existp");
    keywords.insert(type, "instance-keyp");
    keywords.insert(type, "instancep");
    
    //type = "Debugging & Profiling";
    type = "Debugging";
    keywords.insert(type, "get-profile-percent-threshold");
    keywords.insert(type, "profile");
    keywords.insert(type, "profile-info");
    keywords.insert(type, "profile-reset");
    keywords.insert(type, "set-profile-percent-thresholds");
    keywords.insert(type, "dribble-off");
    keywords.insert(type, "dribble-on");
    keywords.insert(type, "list-watch-items");
    keywords.insert(type, "unwatch");
    keywords.insert(type, "watch");
    keywords.insert(type, "conserve-mem");
    keywords.insert(type, "mem-requests");
    keywords.insert(type, "mem-used");
    keywords.insert(type, "release-mem");
    
    type = "Facts";
    keywords.insert(type, "ppdeffacts");
    keywords.insert(type, "list-deffacts");
    keywords.insert(type, "undeffacts");
    keywords.insert(type, "deffacts-module");
    keywords.insert(type, "get-deffacts-list");
    keywords.insert(type, "dependencies");
    keywords.insert(type, "dependents");
    keywords.insert(type, "facts");
    keywords.insert(type, "get-fact-duplication");
    keywords.insert(type, "load-facts");
    keywords.insert(type, "ppfact");
    keywords.insert(type, "save-facts");
    keywords.insert(type, "set-fact-duplication");
    keywords.insert(type, "assert");
    keywords.insert(type, "assert-string");
    keywords.insert(type, "duplicate");
    keywords.insert(type, "fact-existp");
    keywords.insert(type, "fact-index");
    keywords.insert(type, "fact-relation");
    keywords.insert(type, "fact-slot-keys");
    keywords.insert(type, "fact-slot-value");
    keywords.insert(type, "fact-slot-names");
    keywords.insert(type, "get-fact-list");
    keywords.insert(type, "modify");
    keywords.insert(type, "retract");
    
    type = "Functions";
    keywords.insert(type, "get-deffunction-list");
    keywords.insert(type, "deffunction-module");
    keywords.insert(type, "list-deffunctions");
    keywords.insert(type, "ppdeffunction");
    keywords.insert(type, "undeffunction");
    keywords.insert(type, "deffunction-module");
    
    type = "Generics";
    keywords.insert(type, "call-next-method");
    keywords.insert(type, "call-specific-method");
    keywords.insert(type, "defgeneric-module");
    keywords.insert(type, "get-defgeneric-list");
    keywords.insert(type, "get-defmethod-list");
    keywords.insert(type, "get-method-restrictions");
    keywords.insert(type, "next-methodp");
    keywords.insert(type, "override-next-method");
    keywords.insert(type, "list-defgenerics");
    keywords.insert(type, "list-defmethods");
    keywords.insert(type, "ppdefgeneric");
    keywords.insert(type, "ppdefmethod");
    keywords.insert(type, "preview-generic");
    keywords.insert(type, "undefgeneric");
    keywords.insert(type, "undefmethod");
    
    type = "Globals";
    keywords.insert(type, "get-reset-globals");
    keywords.insert(type, "list-defglobals");
    keywords.insert(type, "ppdefglobal");
    keywords.insert(type, "set-reset-globals");
    keywords.insert(type, "show-defglobals");
    keywords.insert(type, "undefglobal");
    keywords.insert(type, "get-defglobal-list");
    keywords.insert(type, "defglobal-module");
    
    type = "IO & Environment";
    keywords.insert(type, "close");
    keywords.insert(type, "format");
    keywords.insert(type, "open");
    keywords.insert(type, "printout");
    keywords.insert(type, "read");
    keywords.insert(type, "readline");
    keywords.insert(type, "remove");
    keywords.insert(type, "rename");
    keywords.insert(type, "rekey");
    keywords.insert(type, "get-char");
    keywords.insert(type, "read-number");
    keywords.insert(type, "set-locale");
    keywords.insert(type, "apropos");
    keywords.insert(type, "batch");
    keywords.insert(type, "batch*");
    keywords.insert(type, "bload");
    keywords.insert(type, "bsave");
    keywords.insert(type, "clear");
    keywords.insert(type, "exit");
    keywords.insert(type, "get-auto-float-dividend");
    keywords.insert(type, "get-dynamic-constraint-checking");
    keywords.insert(type, "get-static-constraint-checking");
    keywords.insert(type, "load");
    keywords.insert(type, "load*");
    keywords.insert(type, "options");
    keywords.insert(type, "reset");
    keywords.insert(type, "save");
    keywords.insert(type, "set-auto-float-dividend");
    keywords.insert(type, "set-dynamic-constraint-checking");
    keywords.insert(type, "set-static-constraint-checking");
    keywords.insert(type, "system");
    
    // !=
    // *
    // **
    // +
    // -/
    // <
    // <=
    // <>
    // =
    // >
    // >=
    type = "Math";
    keywords.insert(type, "abs");
    keywords.insert(type, "div");
    keywords.insert(type, "float");
    keywords.insert(type, "integer");
    keywords.insert(type, "max");
    keywords.insert(type, "min");
    keywords.insert(type, "deg-grad");
    keywords.insert(type, "deg-rad");
    keywords.insert(type, "exp");
    keywords.insert(type, "grad-deg");
    keywords.insert(type, "log");
    keywords.insert(type, "log10");
    keywords.insert(type, "mod");
    keywords.insert(type, "pi");
    keywords.insert(type, "rad-deg");
    keywords.insert(type, "round");
    keywords.insert(type, "sqrt");
    keywords.insert(type, "acos");
    keywords.insert(type, "acosh");
    keywords.insert(type, "acot");
    keywords.insert(type, "acoth");
    keywords.insert(type, "acsc");
    keywords.insert(type, "acsch");
    keywords.insert(type, "asec");
    keywords.insert(type, "asech");
    keywords.insert(type, "asin");
    keywords.insert(type, "asinh");
    keywords.insert(type, "atan");
    keywords.insert(type, "atanh");
    keywords.insert(type, "cos");
    keywords.insert(type, "cosh");
    keywords.insert(type, "cot");
    keywords.insert(type, "coth");
    keywords.insert(type, "csc");
    keywords.insert(type, "csch");
    keywords.insert(type, "sec");
    keywords.insert(type, "sech");
    keywords.insert(type, "sin");
    keywords.insert(type, "sinh");
    keywords.insert(type, "tan");
    keywords.insert(type, "tanh");

    type = "Miscellaneous";
    keywords.insert(type, "funcall");
    keywords.insert(type, "gensym*");
    keywords.insert(type, "gensym");
    keywords.insert(type, "get-function-restrictions");
    keywords.insert(type, "random");
    keywords.insert(type, "seed");
    keywords.insert(type, "sort");
    keywords.insert(type, "setgen");
    keywords.insert(type, "timer");
    keywords.insert(type, "time");
    keywords.insert(type, "constructs-to-c");
    keywords.insert(type, "help");
    keywords.insert(type, "help-path");
    
    type = "Modules";
    keywords.insert(type, "get-current-module");
    keywords.insert(type, "list-defmodules");
    keywords.insert(type, "ppdefmodule");
    keywords.insert(type, "set-current-module");
    keywords.insert(type, "get-defmodule-list");
    keywords.insert(type, "export");
    keywords.insert(type, "import");
    
    type = "Multifields";
    keywords.insert(type, "create$");
    keywords.insert(type, "delete$");
    keywords.insert(type, "delete-member$");
    keywords.insert(type, "explode$");
    keywords.insert(type, "first$");
    keywords.insert(type, "implode$");
    keywords.insert(type, "insert$");
    keywords.insert(type, "length$");
    keywords.insert(type, "member$");
    keywords.insert(type, "member");
    keywords.insert(type, "nth$");
    keywords.insert(type, "nth");
    keywords.insert(type, "replace$");
    keywords.insert(type, "replace-member$");
    keywords.insert(type, "rest$");
    keywords.insert(type, "subseq$");
    keywords.insert(type, "subsetp");
    keywords.insert(type, "subset");
    keywords.insert(type, "slot-direct-delete$");
    keywords.insert(type, "slot-direct-insert$");
    keywords.insert(type, "slot-direct-replace$");
    keywords.insert(type, "sequencep");
    keywords.insert(type, "expand$");
    keywords.insert(type, "get-sequence-operator-recognition");
    keywords.insert(type, "set-sequence-operator-recognition");
    
    type = "Predicates";
    keywords.insert(type, "and");
    keywords.insert(type, "eq");
    keywords.insert(type, "evenp");
    keywords.insert(type, "floatpfloatp");
    keywords.insert(type, "integerp");
    keywords.insert(type, "lexemep");
    keywords.insert(type, "multifieldp");
    keywords.insert(type, "neq");
    keywords.insert(type, "not");
    keywords.insert(type, "numberp");
    keywords.insert(type, "floatp");
    keywords.insert(type, "oddp");
    keywords.insert(type, "or");
    keywords.insert(type, "pointerp");
    keywords.insert(type, "stringp");
    keywords.insert(type, "symbolp");
    keywords.insert(type, "wordp");
    
    type = "Procedurals";
    keywords.insert(type, "bind");
    keywords.insert(type, "break");
    keywords.insert(type, "if");
    keywords.insert(type, "loop-for-count");
    keywords.insert(type, "progn");
    keywords.insert(type, "progn$");
    keywords.insert(type, "return");
    keywords.insert(type, "switch");
    keywords.insert(type, "while");

    type = "Reserved";
    keywords.insert(type, "show-fht");
    keywords.insert(type, "show-fpn");
    keywords.insert(type, "show-joins");
    keywords.insert(type, "show-opn");
    keywords.insert(type, "edit");
    keywords.insert(type, "mv-append");
    keywords.insert(type, "mv-delete");
    keywords.insert(type, "mv-replace");
    keywords.insert(type, "mv-slot-delete");
    keywords.insert(type, "mv-slot-insert");
    keywords.insert(type, "mv-slot-replace");
    keywords.insert(type, "mv-subseq");
    keywords.insert(type, "direct-mv-delete");
    keywords.insert(type, "direct-mv-insert");
    keywords.insert(type, "direct-mv-replace");
    keywords.insert(type, "rule-complexity");
    keywords.insert(type, "rules");
    keywords.insert(type, "primitives-info");
    
    type = "Rules";
    keywords.insert(type, "get-incremental-reset");
    keywords.insert(type, "list-defrules");
    keywords.insert(type, "matches");
    keywords.insert(type, "ppdefrule");
    keywords.insert(type, "refresh");
    keywords.insert(type, "remove-break");
    keywords.insert(type, "set-break");
    keywords.insert(type, "set-incremental-reset");
    keywords.insert(type, "show-breaks");
    keywords.insert(type, "undefrule");
    keywords.insert(type, "get-defrule-list");
    keywords.insert(type, "defrule-module");
    keywords.insert(type, "declare");
    keywords.insert(type, "salience");
    keywords.insert(type, "auto-focus");
    
    type = "String Functions";
    keywords.insert(type, "build");
    keywords.insert(type, "check-syntax");
    keywords.insert(type, "eval");
    keywords.insert(type, "lowcase");
    keywords.insert(type, "string-to-field");
    keywords.insert(type, "str-cat");
    keywords.insert(type, "str-compare");
    keywords.insert(type, "str-index");
    keywords.insert(type, "str-length");
    keywords.insert(type, "sub-string");
    keywords.insert(type, "str-assert");
    keywords.insert(type, "str-explode");
    keywords.insert(type, "str-implode");
    keywords.insert(type, "str-index");
    keywords.insert(type, "sym-cat");
    keywords.insert(type, "upcase");
    keywords.insert(type, "fetch");
    keywords.insert(type, "get-region");
    keywords.insert(type, "print-region");
    keywords.insert(type, "toss");
    keywords.insert(type, "length");       

    type = "Templates";
    keywords.insert(type, "deftemplate-module");
    keywords.insert(type, "deftemplate-slot-allowed-values");
    keywords.insert(type, "deftemplate-slot-cardinality");
    keywords.insert(type, "deftemplate-slot-defaultp");
    keywords.insert(type, "deftemplate-slot-default-value");
    keywords.insert(type, "deftemplate-slot-existp");
    keywords.insert(type, "deftemplate-slot-multip");
    keywords.insert(type, "deftemplate-slot-keys");
    keywords.insert(type, "deftemplate-slot-range");
    keywords.insert(type, "deftemplate-slot-singlep");
    keywords.insert(type, "deftemplate-slot-types");
    keywords.insert(type, "get-deftemplate-list");
    keywords.insert(type, "list-deftemplates");
    keywords.insert(type, "ppdeftemplate");
    keywords.insert(type, "undeftemplate");
    keywords.insert(type, "slot");
    keywords.insert(type, "single-slot");
    keywords.insert(type, "multislot");
    keywords.insert(type, "default"); 
    keywords.insert(type, "default-dynamic");
}
