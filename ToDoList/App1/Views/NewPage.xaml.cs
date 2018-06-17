using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;
using Windows.Storage.Pickers;
using Windows.Storage;
using Windows.Storage.Streams;
using Windows.Storage.AccessCache;
using Windows.UI.Xaml.Media.Imaging;
using Windows.UI.Notifications;
using Windows.Data.Xml.Dom;

// https://go.microsoft.com/fwlink/?LinkId=234238 上介绍了“空白页”项模板

namespace App1.Views
{
    /// <summary>
    /// 可用于自身或导航至 Frame 内部的空白页。
    /// </summary>
    public sealed partial class NewPage : Page
    {   
        public Viewmodels.MainPageVM vm { get; private set; }

        public NewPage()
        {
            this.InitializeComponent();
            DataContextChanged += (s, e) =>
            {
                vm = DataContext as Viewmodels.MainPageVM;
            };
            this.DataContext = App1.Viewmodels.MainPageVM.Instance;
        }
        protected override void OnNavigatedFrom(NavigationEventArgs e)
        {
            bool suspending = ((App)App.Current).issuspend;
            var composite = new ApplicationDataCompositeValue();
            if(suspending == true)
            {
                composite["Title"] = Title.Text;
                composite["Detail"] = Detail.Text;
                composite["Date"] = Date.Date;
                ApplicationData.Current.LocalSettings.Values["NewPage"] = composite;
            }
        }
        protected override async void OnNavigatedTo(NavigationEventArgs e)
        {
            base.OnNavigatedTo(e);
            if(e.NavigationMode != NavigationMode.New)
            {
                if (ApplicationData.Current.LocalSettings.Values.ContainsKey("NewPage"))
                {   
                    if(ApplicationData.Current.LocalSettings.Values["Image"] != null)
                    {
                        StorageFile imagefile = await StorageApplicationPermissions.FutureAccessList.GetFileAsync((string)ApplicationData.Current.LocalSettings.Values["Image"]);
                        IRandomAccessStream stream = await imagefile.OpenAsync(FileAccessMode.Read);
                        var bitmapimage = new BitmapImage();
                        bitmapimage.SetSource(stream);
                        Img.Source = bitmapimage;
                    }
                    var composite = ApplicationData.Current.LocalSettings.Values["NewPage"] as ApplicationDataCompositeValue;
                    Title.Text = (string)composite["Title"];
                    Detail.Text = (string)composite["Detail"];
                    Date.Date = (DateTimeOffset)composite["Date"];
                }
                return;
            }
            if (vm.currentId != -1)
            {
                Title.Text = vm.Items[vm.currentId].topic;
                Detail.Text = vm.Items[vm.currentId].content;
                Date.Date = vm.Items[vm.currentId].date;
                Img.Source = vm.Items[vm.currentId].imageSource;
                CreateBut.Content = "Update";
            }
        }
        private async void Select_Click(object sender, RoutedEventArgs e)
        {
            FileOpenPicker picker = new FileOpenPicker();
            picker.ViewMode = PickerViewMode.Thumbnail;
            picker.SuggestedStartLocation = PickerLocationId.Desktop;
            picker.FileTypeFilter.Add(".jpg");
            picker.FileTypeFilter.Add(".jpeg");
            picker.FileTypeFilter.Add(".png");
            picker.FileTypeFilter.Add(".gif");
            StorageFile file = await picker.PickSingleFileAsync();
            if (file != null)
            {
                ApplicationData.Current.LocalSettings.Values["Image"] = StorageApplicationPermissions.FutureAccessList.Add(file);
                IRandomAccessStream fileStream = await file.OpenAsync(Windows.Storage.FileAccessMode.Read);
                Windows.UI.Xaml.Media.Imaging.BitmapImage bitmapImage = new Windows.UI.Xaml.Media.Imaging.BitmapImage();
                await bitmapImage.SetSourceAsync(fileStream);
                Img.Source = bitmapImage;
            }
        }
        private async void CreateClick(Object sender, RoutedEventArgs e)
        {
            var dialog = new ContentDialog()
            {
                Title = "提示",
                PrimaryButtonText = "确定",
            };
            if (Title.Text == "" || Detail.Text == "") dialog.Content += "Title栏和Detail栏不可为空！\n";
            DateTime now = new DateTime(DateTime.Now.Year, DateTime.Now.Month, DateTime.Now.Day);
            if(Date.Date.CompareTo(now) < 0) dialog.Content += "DueDate不可设为过去的日期！\n";
            if (dialog.Content != null)
            {
                await dialog.ShowAsync();
                return;
            }
            if (vm.currentId == -1) vm.AddItem(Title.Text, Detail.Text, Date.Date, Img.Source);
            else vm.ChangeItem(vm.currentId, Title.Text, Detail.Text, Date.Date, Img.Source);
            vm.currentId = -1;
            //磁贴
            XmlDocument tilexml = new XmlDocument();
            tilexml.LoadXml(File.ReadAllText("Tile.xml"));
            XmlNodeList tileTextAttr = tilexml.GetElementsByTagName("text");
            for (int i = 0; i < tileTextAttr.Count; i++)
            {
                ((XmlElement)tileTextAttr[i]).InnerText = Viewmodels.MainPageVM.Instance.Items[Viewmodels.MainPageVM.Instance.Items.Count - 1].topic;
                i++;
                ((XmlElement)tileTextAttr[i]).InnerText = Viewmodels.MainPageVM.Instance.Items[Viewmodels.MainPageVM.Instance.Items.Count - 1].content;

            }
            TileNotification notifi = new TileNotification(tilexml);
            var updator = TileUpdateManager.CreateTileUpdaterForApplication();
            updator.Update(notifi);
            updator.EnableNotificationQueue(true);
            this.Frame.Navigate(typeof(MainPage));
        }
        private void CancelClick(Object sender, RoutedEventArgs e)
        {
            Title.Text = "";
            Detail.Text = "";
            Date.Date = DateTime.Now;
            this.Frame.Navigate(typeof(MainPage));
        }
        private void Delete_Click(Object s, RoutedEventArgs e)
        {
            vm.RemoveItem(vm.currentId);
            vm.currentId = -1;
            this.Frame.Navigate(typeof(MainPage));
        }
    }
}
