#ifndef ARRAY_AST
#define ARRAY_AST

#include <vector>
#include "../headers/llvm_commons.h"

class ArrayAST : public GenericAST {
    std::vector<std::unique_ptr<GenericAST>> value;

    public:
        ArrayAST(
            std::vector<std::unique_ptr<GenericAST>> value
        ) : value(std::move(value)) {}
        llvm::Value *codegen() {
            std::vector<llvm::Constant*> constElements;
            llvm::Type* elementType = nullptr;

            for (auto& element : elements) {
                llvm::Value* elementValue = element->codegen();
                if (!elementValue) {
                    return nullptr;
                }

                if (!elementType) {
                    elementType = elementValue->getType();
                }

                if (llvm::Constant* constElement = llvm::dyn_cast<llvm::Constant>(elementValue)) {
                    constElements.push_back(constElement);
                } else {
                    return LogErrorValue("Array elements must be constant values");
                }
            }

            llvm::ArrayType* arrayType = llvm::ArrayType::get(elementType, constElements.size());
            return llvm::ConstantArray::get(arrayType, constElements);            
        }

};

#endif