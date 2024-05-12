#ifndef PARSER_HEADERS_H
#define PARSER_HEADERS_H

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "../ast/GenericAST.h"
#include "../ast/ArrayAST.h"
#include "../ast/AddSubOpAST.h"
#include "../ast/MulDivOpAST.h"
#include "../ast/BinaryExpAST.h"
#include "../ast/CharAST.h"
#include "../ast/DoWhileAST.h"
#include "../ast/FileAST.h"
#include "../ast/FloatAST.h"
#include "../ast/ForRangeAST.h"
#include "../ast/ForInAST.h"
#include "../ast/FunctionAST.h"
#include "../ast/FunctionCallAST.h"
#include "../ast/FunctionPrototypeAST.h"
#include "../ast/FunctionPrototypeArgumentAST.h"
#include "../ast/FunctionPrototypeArgumentFloatAST.h"
#include "../ast/FunctionPrototypeArgumentIntAST.h"
#include "../ast/FunctionPrototypeArgumentStringAST.h"
#include "../ast/FunctionPrototypeArgumentCharAST.h"
#include "../ast/ImportAST.h"
#include "../ast/ReturnAST.h"
#include "../ast/GenericAST.h"
#include "../ast/IfAST.h"
#include "../ast/IntAST.h"
#include "../ast/StringAST.h"
#include "../ast/StructAST.h"
#include "../ast/VariableAST.h"
#include "../ast/VariableAssignAST.h"
#include "../ast/WhileAST.h"

#endif