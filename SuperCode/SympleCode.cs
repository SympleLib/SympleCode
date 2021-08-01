namespace SuperCode
{
	public enum SycMode
	{
		Casual,
		Industry,
	}

	public class SympleCode
	{
		public SycMode mode;
		public readonly bool allUnsafe;

		public Visibility defaultVis => mode is SycMode.Industry ? Visibility.Private : Visibility.Public;
		public bool defaultConst => mode is SycMode.Industry;

		public SympleCode(SycMode mode, bool allUnsafe)
		{
			this.mode = mode;
			this.allUnsafe = allUnsafe;
		}
	}
}
