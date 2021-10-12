namespace CommenSense;

using Value = LLVMValueRef;

partial class Builder
{
	Scope scope;

	void EnterScope() =>
		scope = new Scope(this, scope);

	void ExitScope() =>
		scope = scope.parent!;

	class Scope
	{
		public readonly Builder builder;
		public readonly Scope? parent;
		readonly Dictionary<string, StructAst> structs = new Dictionary<string, StructAst>();
		readonly Dictionary<string, ClassAst> classes = new Dictionary<string, ClassAst>();
		readonly Dictionary<string, Value> symbols = new Dictionary<string, Value>();

		public Scope(Builder builder, Scope? parent = null)
		{
			this.builder = builder;
			this.parent = parent;
		}

		public void Define(string name, StructAst ztruct) =>
			structs.Add(name, ztruct);

		public void Define(string name, ClassAst clazz) =>
			classes.Add(name, clazz);

		public StructAst GetStruct(string name)
		{
			if (structs.TryGetValue(name, out StructAst? ztruct))
				return ztruct;
			if (parent is not null)
				return parent!.GetStruct(name);
			throw new Exception("struct don't exist man");
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
			symbols.Add(name, symbol);
	}
}