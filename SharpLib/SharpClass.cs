using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.IO;

namespace SharpLib
{
    public class SharpClass
    {
        public int Number { get; set; }
        public string DownloadUrl(string url)
        {
            return new WebClient().DownloadString(url);
        }

        public void DownloadUrlAsync(string url, Action<string, IntPtr> callback, IntPtr state)
        {
            var request = WebRequest.Create(url);
            request.BeginGetResponse((ar) =>
            {
                var response = request.EndGetResponse(ar);
                using (var reader = new StreamReader(response.GetResponseStream()))
                {
                    callback(reader.ReadToEnd(), state);
                }
            }, null);
        }

        public SharpClass()
        {
            Number = 42;
        }
    }
}
