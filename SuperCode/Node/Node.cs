using System;
using System.IO;
using System.Collections.Generic;

namespace SuperCode
{
	public abstract class Node
	{
		public Ast syntax { get; init; }
		public abstract NodeKind kind { get; }
	}
}
