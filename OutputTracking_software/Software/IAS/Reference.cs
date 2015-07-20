using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace IAS
{
    public class Reference
    {
        public String Name { get; set; }
        public int CycleTime { get; set; }
        public int BottleNeckTime { get; set; }
        public String Code {get;set;}

        public Reference()
        {
        }
    }
}
