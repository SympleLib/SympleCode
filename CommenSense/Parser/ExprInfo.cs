namespace CommenSense;

partial class Parser
{
	static readonly TokenKind[] preOps = {
		TokenKind.Plus,
		TokenKind.Minus,

		TokenKind.And,
		// TokenKind.Percent,
	};

	static Enum PreOpcode(TokenKind kind) =>
		kind switch
		{
			TokenKind.And => kind,
			TokenKind.Plus => LLVMAdd,
			TokenKind.Minus => LLVMFNeg,
			TokenKind.Percent => LLVMLoad,
			_ => (LLVMOpcode) 0,
		};

	static readonly TokenKind[] postOps = {
		TokenKind.Plus2,
		TokenKind.Minus2,
		TokenKind.Bang2,
		TokenKind.Tilde2,
	};

	static Enum PostOpcode(TokenKind kind) =>
		kind switch {
			TokenKind.Plus2 => LLVMAdd,
			TokenKind.Minus2 => LLVMSub,
			TokenKind.Bang2 => kind,
			TokenKind.Tilde2 => LLVMXor,
			_ => (LLVMOpcode) 0,
		};

	static int BiPrecendence(TokenKind kind) =>
		   kind switch
		   {
			   TokenKind.Dot => 9,
			   TokenKind.Star2 => 8,
			   TokenKind.Star or TokenKind.Slash or TokenKind.Percent or TokenKind.SlashDash or TokenKind.PercentDash => 7,
			   TokenKind.Plus or TokenKind.Minus => 6,
			   TokenKind.Left2 or TokenKind.Right2 => 5,
			   TokenKind.And or TokenKind.Pipe or TokenKind.Carot => 4,
			   TokenKind.LeftChevron or TokenKind.RightChevron or TokenKind.LEql or TokenKind.REql or TokenKind.EqlEql or TokenKind.TildeEql or TokenKind.BangEql => 3,
			   TokenKind.And2 or TokenKind.Pipe2 => 2,
			   TokenKind.Eql or TokenKind.PlusEql or TokenKind.MinusEql or TokenKind.StarEql or TokenKind.SlashEql or TokenKind.PercentEql or
			   TokenKind.Star2Eql or TokenKind.Left2Eql or TokenKind.Right2Eql or
			   TokenKind.AndEql or TokenKind.PipeEql or TokenKind.CarotEql => 1,
			   _ => 0,
		   };

	static Enum BiOpcode(TokenKind kind) =>
		kind switch
		{
			TokenKind.Eql or TokenKind.PlusEql or TokenKind.MinusEql or TokenKind.StarEql or TokenKind.SlashEql or TokenKind.PercentEql or
			TokenKind.Star2Eql or TokenKind.Left2Eql or TokenKind.Right2Eql or
			TokenKind.TildeEql or
			TokenKind.And2 or TokenKind.Pipe2 or
			TokenKind.Star2 or
			TokenKind.AndEql or TokenKind.PipeEql or TokenKind.CarotEql or
			TokenKind.Dot => kind,

			TokenKind.Plus => LLVMAdd,
			TokenKind.Minus => LLVMSub,
			TokenKind.Star => LLVMMul,
			TokenKind.Slash => LLVMSDiv,
			TokenKind.Percent => LLVMSRem,
			TokenKind.SlashDash => LLVMUDiv,
			TokenKind.PercentDash => LLVMURem,

			TokenKind.Left2 => LLVMShl,
			TokenKind.Right2 => LLVMAShr,

			TokenKind.And => LLVMAnd,
			TokenKind.Pipe => LLVMOr,
			TokenKind.Carot => LLVMXor,

			TokenKind.LeftChevron => LLVMIntSLT,
			TokenKind.RightChevron => LLVMIntSGT,
			TokenKind.LEql => LLVMIntSLE,
			TokenKind.REql => LLVMIntSGE,
			TokenKind.EqlEql => LLVMIntEQ,
			TokenKind.BangEql => LLVMIntNE,
			_ => (LLVMOpcode) 0,
		};
}