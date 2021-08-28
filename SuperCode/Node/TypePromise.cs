using LLVMSharp.Interop;

namespace SuperCode
{
	public class TypePromise
	{
		public readonly TypePrompt prompt;
		public LLVMTypeRef? gift => prompt.realType;

		public TypePromise(TypePrompt prompt) =>
			this.prompt = prompt;
	}

	public interface TypePrompt
	{ public LLVMTypeRef? realType { get; } }
}
