namespace CommenSense;

partial class Parser
{
	static readonly TokenKind[] unOps = {
		TokenKind.Plus,
		TokenKind.Minus,

		TokenKind.And,
		// TokenKind.Percent,
	};

	static Enum UnOpcode(TokenKind kind) =>
		kind switch
		{
			TokenKind.And => kind,
			TokenKind.Plus => LLVMAdd,
			TokenKind.Minus => LLVMFNeg,
			TokenKind.Percent => LLVMLoad,
			_ => (LLVMOpcode) 0,
		};

	static int BiPrecendence(TokenKind kind) =>
		   kind switch
		   {
			   TokenKind.Dot => 5,
			   TokenKind.Star or TokenKind.Slash or TokenKind.Percent or TokenKind.SlashDash or TokenKind.PercentDash => 4,
			   TokenKind.Plus or TokenKind.Minus => 3,
			   TokenKind.LeftChevron or TokenKind.RightChevron or TokenKind.LEql or TokenKind.REql => 2,
			   TokenKind.Eql or TokenKind.PlusEql or TokenKind.MinusEql or TokenKind.StarEql or TokenKind.SlashEql or TokenKind.PercentEql => 1,
			   _ => 0,
		   };

	static Enum BiOpcode(TokenKind kind) =>
		kind switch
		{
			TokenKind.Eql or TokenKind.PlusEql or TokenKind.MinusEql or TokenKind.StarEql or TokenKind.SlashEql or TokenKind.PercentEql
				or TokenKind.Dot => kind,
			TokenKind.Plus => LLVMAdd,
			TokenKind.Minus => LLVMSub,
			TokenKind.Star => LLVMMul,
			TokenKind.Slash => LLVMSDiv,
			TokenKind.Percent => LLVMSRem,
			TokenKind.SlashDash => LLVMUDiv,
			TokenKind.PercentDash => LLVMURem,

			TokenKind.LeftChevron => LLVMIntSLT,
			TokenKind.RightChevron => LLVMIntSGT,
			TokenKind.LEql => LLVMIntSLE,
			TokenKind.REql => LLVMIntSGE,
			_ => (LLVMOpcode) 0,
		};
}