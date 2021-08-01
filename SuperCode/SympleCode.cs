﻿namespace SuperCode
{
	public enum SycMode
	{
		DefaultPrivate = 1 << 0,
		DefaultConstant = 1 << 1,
		AllUnsafe = 1 << 2,

		Default = 0,
		Industry = DefaultPrivate | DefaultConstant,
	}

	public class SympleCode
	{
		public SycMode mode;

		public bool defPriv => mode.HasFlag(SycMode.DefaultPrivate);
		public bool defConst => mode.HasFlag(SycMode.DefaultConstant);
		public bool allUnsafe => mode.HasFlag(SycMode.AllUnsafe);

		public Visibility defVis => defPriv ? Visibility.Private : Visibility.Public;

		public SympleCode(SycMode mode) =>
			this.mode = mode;
	}
}
