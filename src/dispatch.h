#include <map>
#include <string>

#include "llvm/Pass.h"
#include "llvm/Function.h"
#include "llvm/BasicBlock.h"
#include "llvm/Instructions.h"

using namespace llvm;

class MakeDispatcherPass : public FunctionPass
{
 public:
  static char ID;
 MakeDispatcherPass() : FunctionPass(ID) {}
  virtual bool runOnFunction( Function& currFunction );

 private:
  static bool IsUsedOutsideParentBlock( Instruction* value );
  static void ConvertSwitch( Function& function );


  static const std::string PassName;
};
