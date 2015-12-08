using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.ServiceModel.Web;
using System.Text;

namespace PokerSvc
{
    public class OpenPoker : IOpenPoker
    {
        public string TestServiceIsAlive(int value)
        {
            return string.Format("You entered: {0}", value);
        }
    }
}
