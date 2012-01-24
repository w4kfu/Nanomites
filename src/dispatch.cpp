#include <map>
#include <vector>
#include <cassert>
#include <iostream>
#include <exception>

#include "llvm/Type.h"
#include "llvm/Module.h"
#include "llvm/Function.h"
#include "llvm/Constants.h"
#include "llvm/Instructions.h"
#include "llvm/Support/CFG.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/InstIterator.h"

#include "dispatch.h"

// MakeDispatcherPass::MakeDispatcherPass()
// {
//   FunctionPass(ID);
// }
char MakeDispatcherPass::ID = 0;
const std::string MakeDispatcherPass::PassName = "MakeDispatcherPass";

bool MakeDispatcherPass::runOnFunction( Function& currFunction )
{
  BasicBlock* entryBB = &currFunction.getEntryBlock();
  typedef std::vector< Instruction * > InstList;
  InstList insts;
  bool madeChange = true;


  // std::cout << PassName << ":Processing " << currFunction.getName();

  // std::cout << ":Processing " << currFunction.getName().str() << std::endl;

  // for( inst_iterator i = inst_begin( currFunction ); i != inst_end( currFunction
  // 								); i++ )
  //   {
  //     Instruction* inst = &*i;

  //     std::cout << "Opcode = " << inst->getOpcodeName() << std::endl;
  //     if( IsUsedOutsideParentBlock( inst ) && !isa< AllocaInst >( inst ) )
  //       {
  // 	  std::cout << "IsUsedOutsideParentBlock" << std::endl;
  // 	  insts.push_back( inst );
  //       }
  //   }

  // for( InstList::iterator i = insts.begin(); i != insts.end(); i++ )
  //   {
  //     std::cout << "LOL" << std::endl;
  //   }

  ConvertSwitch(currFunction);
  return (true);
}

bool MakeDispatcherPass::IsUsedOutsideParentBlock( Instruction* value )
{
  for( Value::use_iterator i = value->use_begin(); i != value->use_end();
       i++ )
    {
      Instruction* user = dynamic_cast< Instruction* >( *i );
      if( user->getParent() != value->getParent() )
        {
	  return true;
        }
    }
  return false;
}


void MakeDispatcherPass::ConvertSwitch( Function& function )
{
  BasicBlock* entryBB = &function.getEntryBlock();

  std::cout << ":Processing " << function.getName().str() << std::endl;

  for( Function::iterator i = function.begin(); i != function.end(); i++ )
    {
      BasicBlock* basicBlock = &*i;

      TerminatorInst* terminator = basicBlock->getTerminator();
      assert( terminator && "Basic block is not well formed and has no terminator!" );

      if( isa< BranchInst >( terminator ) )
        {
	  // std::cout << "Branch Instruction !!!" << std::endl;
	  BranchInst* branchInst = dynamic_cast< BranchInst *>
	    (
	     basicBlock->getTerminator()
	     );
	  std::cout << "Branch Instruction, Opcode = " <<
	    branchInst->getOpcodeName();
	  if (branchInst->isConditional())
	    {
	      std::cout << ", Is Conditional" << std::endl;
	    }
	  else
	    {
	      std::cout << ", Is not conditionnal" << std::endl;
	    }
	  // SwitchInst* switchInst = dynamic_cast< SwitchInst * >(
	  // 							basicBlock->getTerminator()
        // );

	  // Allocate a local variable and add an instruction saving the value
	  // being tested by the switch instruction to this variable.
	  //
	  // Allocating of the new variable is necessary because initial order
	  // of the basic blocks will be changed by dispatcher embedding.
	  // AllocaInst* testValuePtr = new AllocaInst(
	  // 					    switchInst->getCondition()->getType(),
	  // 					    0, "switch_test_val",
	  // 					    entryBB->getTerminator() );
	  // new StoreInst( switchInst->getCondition(), testValuePtr, false,
	  // 		 switchInst );

	  // Replace the switch instruction by basic blocks with conditional branches.
	  // BasicBlock* caseBlock = ProcessCase( switchInst, 1, basicBlock,
	  // 				       testValuePtr );
	  // switchInst->eraseFromParent();

	  // new BranchInst( caseBlock, basicBlock );
        }
    }
}
