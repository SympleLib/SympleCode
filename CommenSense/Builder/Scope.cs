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
		readonly Dictionary<string, Value> vars = new Dictionary<string, Value>();
		readonly Dictionary<string, Value> funcs = new Dictionary<string, Value>();
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

		public Value FindVar(string name)
		{
			if (vars.TryGetValue(name, out Value var))
				return var;
			if (parent is not null)
				return parent.FindVar(name);
			throw new Exception("symbol don't exist man");
		}

		public Value FindFunc(string name)
		{
			if (funcs.TryGetValue(name, out Value func))
				return func;
			if (parent is not null)
				return parent.FindFunc(name);
			throw new Exception("symbol don't exist man");
		}

		public void DefineVar(string name, Value symbol) =>
			vars.TryAdd(name, symbol);

		public void DefineFunc(string name, Value func) =>
			funcs.TryAdd(name, func);
	}
}