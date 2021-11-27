namespace CommenSense;

using Value = LLVMValueRef;
using Type = LLVMTypeRef;
using Block = LLVMBasicBlockRef;

partial class Builder
{
	Scope globalScope;
	Scope scope;
	void EnterScope(Block? exit) =>
		scope = new Scope(this, exit, scope);

	void ExitScope() =>
		scope = scope.parent!;

	class Scope
	{
		public readonly Builder builder;
		public readonly Scope? parent;

		public readonly Block? exit;
		public readonly int depth;

		readonly Dictionary<string, Container> ctnrs = new Dictionary<string, Container>();
		readonly Dictionary<string, Value> symbols = new Dictionary<string, Value>();
		readonly Dictionary<string, Type> typedefs = new Dictionary<string, Type>();

		public Scope(Builder builder, Block? exit, Scope? parent = null)
		{
			this.builder = builder;
			this.parent = parent;
			this.exit = exit;
			depth = parent is null ? 0 : parent.depth + 1;
		}

		public void Define(string alias, Type realType) =>
			typedefs.TryAdd(alias, realType);

		public void Define(string name, Container ctnr) =>
			ctnrs.TryAdd(name, ctnr);

		public Container GetCtnr(string name)
		{
			if (ctnrs.TryGetValue(name, out Container? ctnr))
				return ctnr!;
			if (typedefs.TryGetValue(name, out Type ty) && ctnrs.TryGetValue(ty.StructName, out ctnr))
				return ctnr!;
			if (parent is not null)
				return parent!.GetCtnr(name);
			throw new Exception("ctnr don't exist man");
		}

		public Type FindType(string name)
		{
			if (typedefs.TryGetValue(name, out Type type))
				return type;
			if (parent is not null)
				return parent!.FindType(name);

			type = builder.llModule.GetTypeByName(name);
			if (type.Handle != IntPtr.Zero)
				return type;

			throw new Exception("type don't exist man");
		}

		public Value Find(string name)
		{
			if (symbols.TryGetValue(name, out Value symbol))
				return symbol;
			if (parent is not null)
				return parent!.Find(name);
			throw new Exception("symbol don't exist man");
		}

		public void Define(string name, Value symbol) =>
			symbols.TryAdd(name, symbol);
	}
}