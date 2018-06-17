using System;
using System.Collections.ObjectModel;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using App1.Models;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Media.Imaging;
using Windows.UI.Xaml.Controls;
using System.Xml.Serialization;
using System.IO;
using System.Runtime.Serialization;
using System.Xml;

namespace App1.Viewmodels
{
    public class MainPageVM
    {
        public int currentId
        {
            get; set;
        }
        private MainPageVM()
        {
            currentId = -1;
        }
        private static MainPageVM ins;
        public static MainPageVM Instance
        {
            get
            {
                if (ins != null) return ins;
                else
                {
                    ins = new MainPageVM();
                    return ins;
                }
            }
            private set { }
        }
        public ObservableCollection<Models.Item> Items { get; set; } = new ObservableCollection<Models.Item>() {
            new Item(0, "AAAA Thing", "what to do", DateTimeOffset.Now, new BitmapImage(new Uri("ms-appx:///Assets/icon.jpg"))),
            new Item(0, "oooo..", "to do", DateTimeOffset.Now, new BitmapImage(new Uri("ms-appx:///Assets/icon.jpg")))
        };
        public void AddItem(string top, string con, DateTimeOffset date, ImageSource img)
        {
            Item it = new Item(Items.Count(), top, con, date, img);
            Items.Add(it);
        }
        public void ChangeItem(int id, string top, string con, DateTimeOffset date, ImageSource img)
        {
            Items[id].topic = top;
            Items[id].content = con;
            Items[id].date = date;
            Items[id].imageSource = img;
        }
        public void RemoveItem(int id_)
        {
            if (id_ != -1)
            {
                Items.RemoveAt(id_);
                for (int i = id_; i < Items.Count(); i++)
                {
                    Items[i].id--;
                }
            }
        }
        /*public static string serialize()
        {
            var serializer = new DataContractSerializer(typeof(ObservableCollection<Models.Item>));
            var xmlwriter = new stringStream();
            serializer.WriteObject(xmlwriter, MainPageVM.Instance.Items);
            return stringwriter.ToString();
        }
        public static ObservableCollection<Models.Item> deserialize(string xml)
        {
            var serializer = new XmlSerializer(typeof(MainPageVM));
            var stringreader = new StringReader(xml);
            return (ObservableCollection<Models.Item>)serializer.Deserialize(stringreader);
        }*/
    }
}
