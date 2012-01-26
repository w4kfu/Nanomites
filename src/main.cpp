#include <iostream>
#include <fstream>
#include <memory>

#include "llvm/Support/CommandLine.h"
#include "llvm/Assembly/Parser.h"
#include "llvm/LLVMContext.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Module.h"
#include "llvm/PassManager.h"
#include "llvm/Target/TargetData.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_os_ostream.h"
#include "llvm/Assembly/PrintModulePass.h"

#include "dispatch.h"

using namespace llvm;

static cl::opt< std::string >
InputFilename( cl::Positional, cl::desc( "<input .bc file>" ), cl::Required
	       );

static cl::opt< std::string >
OutputFilename( cl::Positional, cl::desc( "<output .bc file>" ),
		cl::Required );

static cl::opt<bool>
DisableVerify("disable-verify", cl::Hidden,
              cl::desc("Do not run verifier on input LLVM (dangerous!)"));

static cl::opt<bool>
DumpAsm("d", cl::desc("Print assembly as parsed"), cl::Hidden);

LLVMContext &Context = getGlobalContext();

int main(int argc, char* argv[])
{
  std::string programName = argv[0];

    try
      {
	cl::ParseCommandLineOptions( argc, argv );
	SMDiagnostic Err;

        // Load the input module.
	std::auto_ptr<Module> M(ParseAssemblyFile(InputFilename, Err, Context));
        if (M.get() == 0)
	  {
	    Err.Print(argv[0], errs());
            return (1);
	  }
	if (!DisableVerify) {
	  std::string Err;
	  if (verifyModule(*M.get(), ReturnStatusAction, &Err)) {
	    errs() << argv[0]
		   << ": assembly parsed, but does not verify as correct!\n";
	    errs() << Err;
	    return (1);
	  }
	}
	std::ostream *Out = new std::ofstream(OutputFilename.c_str(),
					      std::ios::out | std::ios::trunc
					      | std::ios::binary);
        if (!Out->good())
	  {
	    std::cerr << programName << ": Error opening " << OutputFilename
		      << "!\n";
            return (1);
	  }


	if (DumpAsm)
	  errs() << "Here's the assembly:\n" << *M.get();

	PassManager Passes;

	raw_os_ostream L(*Out);

        // Add an appropriate TargetData instance for this module.
        Passes.add(new TargetData("onfuscator"));

        Passes.add(new MakeDispatcherPass());

	// Passes.add(new WriteBytecodePass(Out, true));
	Passes.add(createPrintModulePass(&L));
	Passes.run( *M.get() );
      }
    catch( std::exception e )
      {
	std::cerr << programName << ": " << e.what() << "\n";
        return (1);
      }
    catch( const std::string& msg )
      {
	std::cerr << programName << ": " << msg << "\n";
        return (1);
      }
    catch( ... )
      {
	std::cerr << programName << ": Unexpected exception occurred.\n";
        return (1);
      }

  return (0);
}
