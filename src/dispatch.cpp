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
#include "llvm/InstrTypes.h"
#include "llvm/ValueSymbolTable.h"

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
  bool madeChange = true;


  std::cout << ":Processing " << currFunction.getName().str() << std::endl;

  ConvertCmp(currFunction);

  // for( InstList::iterator i = insts.begin(); i != insts.end(); i++ )
  //   {
  //     std::cout << "LOL" << std::endl;
  //   }

  // ConvertSwitch(currFunction);
  return (true);
}

void ShowType(CmpInst* inst)
{
  switch(inst->getPredicate())
    {
    case CmpInst::ICMP_EQ:
      std::cout << "Equal" << std::endl;
      break;
    case CmpInst::ICMP_NE:
      std::cout << "Not Equal" << std::endl;
      break;
    case CmpInst::ICMP_UGT:
      std::cout << "Unsigned Greater Than" << std::endl;
      break;
    case CmpInst::ICMP_UGE:
      std::cout << "Unsigned Greater Or Equal" << std::endl;
      break;
    case CmpInst::ICMP_ULT:
      std::cout << "Unsigned Less Than" << std::endl;
      break;
    case CmpInst::ICMP_ULE:
      std::cout << "unsigned less or equal" << std::endl;
      break;
    case CmpInst::ICMP_SGT:
      std::cout << "signed greater than" << std::endl;
      break;
    case CmpInst::ICMP_SGE:
      std::cout << "signed greater or equal" << std::endl;
      break;
    case CmpInst::ICMP_SLT:
      std::cout << "signed less than" << std::endl;
      break;
    case CmpInst::ICMP_SLE:
      std::cout << "signed less or equal" << std::endl;
      break;
    default:
      std::cout << "default cmp" << std::endl;
    }
}

void MakeDispatcherPass::ConvertCmp(Function& function)
{
  typedef std::vector< Instruction * > InstList;
  InstList insts;

  for (inst_iterator i = inst_begin( function ); i != inst_end(function); i++)
    {
      Instruction* inst = &*i;

      // std::cout << "Opcode = " << inst->getOpcodeName() << std::endl;
      if (isa< CmpInst>(inst))
	{
	  std::cout << "Compare Instruction : ";
	  ShowType(dynamic_cast<CmpInst*>(inst));
	  insts.push_back(inst);
	}
      if (isa< BranchInst >(inst) && !insts.empty())
	{
	  BranchInst* branchInst = dynamic_cast< BranchInst *>(inst);
	  Value*      val;
	  BasicBlock* IfTrue;
	  BasicBlock* IfFalse;

	  // std::cout << " Nb Successor : " << branchInst->getNumSuccessors();

	  IfTrue = branchInst->getSuccessor(0);
	  IfFalse = branchInst->getSuccessor(1);

	  IfTrue->dump();
	  IfFalse->dump();

	  val = branchInst->getCondition();
	  if (val)
	    {
	      // std::cout <<
		// val->dump();
	      // std::cout << "OL" << std::endl;
	    }

	  std::cout << std::endl;
	  insts.pop_back();
	}



      // if( IsUsedOutsideParentBlock( inst ) && !isa< AllocaInst >( inst ) )
      //   {
      // 	  std::cout << "IsUsedOutsideParentBlock" << std::endl;
      // 	  insts.push_back( inst );
      //   }
    }
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
	      std::cout << ", Is Conditional ";
	    }
	  else
	    {
	      std::cout << ", Is not conditionnal ";
	    }
	  Value*	valbranch = NULL;

	  valbranch = branchInst->getCondition();

	  // if (valbranch) // && !valbranch->getName().empty())
	  //   {
	  //     valbranch->getType()->dump();
	  //     std::cout << valbranch->getName().str();
	  //   }

	  std::cout << std::endl;

	  // std::cout << branchInst->getCondition()->getName.str() << std::endl;
        }
    }
}
