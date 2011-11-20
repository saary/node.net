using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;

namespace SharpLib
{
    public class SharpClass
    {
        public int Number { get; set; }
        public string DownloadUrl(string url)
        {
            return new WebClient().DownloadString(url);
        }

        public SharpClass()
        {
            Number = 42;
        }
    }
}
