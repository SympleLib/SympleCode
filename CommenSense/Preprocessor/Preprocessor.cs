using System.IO;

namespace CommenSense;

class Preprocessor
{
	readonly string folder;
	readonly string filename;

	public Preprocessor(string source, string filename)
	{
		this.filename = filename;
		folder = Path.GetFileName(Path.GetDirectoryName(this.filename)!);
	}
