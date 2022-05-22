/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#pragma once

#include <llvm/ADT/StringRef.h>
#include <llvm/ExecutionEngine/JITSymbol.h>
#include <llvm/ExecutionEngine/Orc/CompileUtils.h>
#include <llvm/ExecutionEngine/Orc/Core.h>
#include <llvm/ExecutionEngine/Orc/ExecutionUtils.h>
#include <llvm/ExecutionEngine/Orc/ExecutorProcessControl.h>
#include <llvm/ExecutionEngine/Orc/IRCompileLayer.h>
#include <llvm/ExecutionEngine/Orc/JITTargetMachineBuilder.h>
#include <llvm/ExecutionEngine/Orc/RTDyldObjectLinkingLayer.h>
#include <llvm/ExecutionEngine/SectionMemoryManager.h>
#include <llvm/IR/DataLayout.h>
#include <llvm/IR/LLVMContext.h>

namespace syc::emit {
	using namespace llvm;
	using namespace llvm::orc;

	class SycJIT {
	private:
		std::unique_ptr<ExecutionSession> session;

		DataLayout dataLayout;
		MangleAndInterner mangle;

		RTDyldObjectLinkingLayer objLayer;
		IRCompileLayer compileLayer;

		JITDylib &mainJd;

	public:
		SycJIT(std::unique_ptr<ExecutionSession> session, JITTargetMachineBuilder builder, DataLayout dataLayout):
			session(std::move(session)), dataLayout(std::move(dataLayout)), mangle(*this->session, this->dataLayout),
			objLayer(*this->session, []() { return std::make_unique<SectionMemoryManager>(); }),
			compileLayer(*this->session, objLayer, std::make_unique<ConcurrentIRCompiler>(std::move(builder))),
			mainJd(this->session->createBareJITDylib("<main>")) {
			mainJd.addGenerator(cantFail(DynamicLibrarySearchGenerator::GetForCurrentProcess(dataLayout.getGlobalPrefix())));
		}

		~SycJIT() {
			if (Error err = session->endSession())
				session->reportError(std::move(err));
		}

		static Expected<std::unique_ptr<SycJIT>> Create() {
			auto epc = SelfExecutorProcessControl::Create();
			if (!epc)
				return epc.takeError();

			auto es = std::make_unique<ExecutionSession>(std::move(*epc));

			JITTargetMachineBuilder builder(es->getExecutorProcessControl().getTargetTriple());

			auto dataLayout = builder.getDefaultDataLayoutForTarget();
			if (!dataLayout)
				return dataLayout.takeError();

			return std::make_unique<SycJIT>(std::move(es), std::move(builder), std::move(*dataLayout));
		}

		Error addModule(ThreadSafeModule module, ResourceTrackerSP rt = nullptr) {
			if (!rt)
				rt = mainJd.getDefaultResourceTracker();
			return compileLayer.add(rt, std::move(module));
		}

		Expected<JITEvaluatedSymbol> lookup(StringRef name) {
			return session->lookup({&mainJd}, mangle(name.str()));
		}
	};
} // syc
