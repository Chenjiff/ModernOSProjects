using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.UI.Xaml.Data;

namespace App1
{
    public class IsCheckedToVisibilityConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, string language)
        {
            bool boolnum = bool.Parse(value.ToString());
            if (boolnum) return Windows.UI.Xaml.Visibility.Visible;
            else return Windows.UI.Xaml.Visibility.Collapsed;
        }
        public object ConvertBack(object value, Type targetType, object parameter, string language)
        {
            throw new NotImplementedException();
        }
    }
}
