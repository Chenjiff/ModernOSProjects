using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using Windows.UI.Xaml.Media;

namespace App1.Models
{
    public class Item : INotifyPropertyChanged
    {
        private int _id;
        private string _topic;
        private string _content;
        private DateTimeOffset _date;
        private bool _iscomp;
        private ImageSource _imagesource;
        public event PropertyChangedEventHandler PropertyChanged;
        internal void OnPropertyChanged([CallerMemberName] string member = "")
        {
            if (PropertyChanged != null)
            {
                PropertyChanged(this, new PropertyChangedEventArgs(member));
            }
        }
        public int id
        {
            get
            {
                return _id;
            }
            set
            {
                _id = value;
                OnPropertyChanged();
            }
        }
        public string topic
        {
            get
            {
                return _topic;
            }
            set
            {
                _topic = value;
                OnPropertyChanged();
            }
        }
        public string content
        {
            get
            {
                return _content;
            }
            set
            {
                _content = value;
                OnPropertyChanged();
            }
        }
        public System.DateTimeOffset date
        {
            get
            {
                return _date;
            }
            set
            {
                _date = value;
                OnPropertyChanged();
            }
        }
        public bool IsCompleted
        {
            get
            {
                return _iscomp;
            }
            set
            {
                _iscomp = value;
                OnPropertyChanged();
            }
        }
        public ImageSource imageSource
        {
            get
            {
                return _imagesource;
            }
            set
            {
                _imagesource = value;
                OnPropertyChanged();
            }
        }

        public Item(int id, string top, string con, DateTimeOffset date, ImageSource img)
        {
            this.id = id;
            this.topic = top;
            this.content = con;
            this.date = date;
            IsCompleted = false;
            this.imageSource = img;
        }
    }
}
