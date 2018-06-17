using System;
using System.IO;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Navigation;
using Windows.Storage.Pickers;
using Windows.Storage;
using Windows.Storage.Streams;
using Windows.Storage.AccessCache;
using Windows.UI.Xaml.Media.Imaging;
using Windows.UI.Notifications;
using Windows.Data.Xml.Dom;
using Windows.ApplicationModel.DataTransfer;
using System.Collections.Generic;

// https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x804 上介绍了“空白页”项模板

namespace App1.Views
{
    /// <summary>
    /// 可用于自身或导航至 Frame 内部的空白页。
    /// </summary>
    public sealed partial class MainPage : Page
    {
        
        public MainPage()
        {
            this.InitializeComponent();
            DataContextChanged += (s, e) =>
            {
                ViewModels = DataContext as Viewmodels.MainPageVM;
            };
            this.DataContext = Viewmodels.MainPageVM.Instance;
        }
        protected override void OnNavigatedFrom(NavigationEventArgs e)
        {
            bool suspending = ((App)App.Current).issuspend;
            if(suspending == true)
            {
                var composite = new ApplicationDataCompositeValue();
                composite["Title"] = Title.Text;
                composite["Detail"] = Detail.Text;
                composite["Date"] = Date.Date;
                composite["Complete0"] = Viewmodels.MainPageVM.Instance.Items[0].IsCompleted;
                composite["Complete1"] = Viewmodels.MainPageVM.Instance.Items[1].IsCompleted;
                ApplicationData.Current.LocalSettings.Values["MainPage"] = composite;
            }
        }
        protected override async void OnNavigatedTo(NavigationEventArgs e)
        {
            if (e.NavigationMode != NavigationMode.New)
            {
                if (ApplicationData.Current.LocalSettings.Values.ContainsKey("MainPage"))
                {   
                    var composite = ApplicationData.Current.LocalSettings.Values["MainPage"] as ApplicationDataCompositeValue;
                    if(ApplicationData.Current.LocalSettings.Values["Image"] != null)
                    {
                        StorageFile imagefile = await StorageApplicationPermissions.FutureAccessList.GetFileAsync((string)ApplicationData.Current.LocalSettings.Values["Image"]);
                        IRandomAccessStream filestream = await imagefile.OpenAsync(FileAccessMode.Read);
                        var bitmapimage = new BitmapImage();
                        bitmapimage.SetSource(filestream);
                        Img.Source = bitmapimage;
                        ApplicationData.Current.LocalSettings.Values["Image"] = null;
                    }
                    Title.Text = (string)composite["Title"];
                    Detail.Text = (string)composite["Detail"];
                    Date.Date = (DateTimeOffset)composite["Date"];
                    Viewmodels.MainPageVM.Instance.Items[0].IsCompleted = (bool)composite["Complete0"];
                    Viewmodels.MainPageVM.Instance.Items[1].IsCompleted = (bool)composite["Complete1"];
                    ApplicationData.Current.LocalSettings.Values.Remove("MainPage");
                }
            }
        }
        public Viewmodels.MainPageVM ViewModels { get; set; }

        private void Item_Click(object sender, ItemClickEventArgs e)
        {   
            //跳转或者显示到侧栏
            var item = e.ClickedItem as Models.Item;
            ViewModels.currentId = item.id;
            if (InternalEdit.Visibility == Visibility.Visible)
            {
                Title.Text = item.topic;
                Detail.Text = item.content;
                Date.Date = item.date;
                CreateBut.Content = "Update";
                Img.Source = item.imageSource;
                return;
            }
            this.Frame.Navigate(typeof(NewPage), " ");
        }
        private void Setting_Click(object s, RoutedEventArgs e)
        {
            //点击设置按钮时修改SelectedIndex为当前点击对象
            var App = s as AppBarButton;
            var Grid = App.Parent as Grid;
            var IDTB = Grid.Children[0] as TextBlock;
            ListView.SelectedIndex = int.Parse(IDTB.Text);
        }

        private void Edit_Click(object sender, RoutedEventArgs e)
        {   
            //跳转到编辑界面或者到侧栏
            var item = ListView.SelectedItem as Models.Item;
            ViewModels.currentId = item.id;
            if (InternalEdit.Visibility == Visibility.Visible) {
                Title.Text = item.topic;
                Detail.Text = item.content;
                Date.Date = item.date;
                Img.Source = item.imageSource;
                CreateBut.Content = "Update";
                return;
            }
            this.Frame.Navigate(typeof(NewPage), " ");
        }

        private void Delete_Click(object sender, RoutedEventArgs e)
        {   
            //删除当前选中项，若右侧栏为当前选中项则初始化右侧栏
            var item = ListView.SelectedItem as Models.Item;
            int id = item.id;
            ViewModels.RemoveItem(id);
            if (InternalEdit.Visibility == Visibility.Visible && ViewModels.currentId == id)
            {
                Title.Text = "";
                Detail.Text = "";
                Date.Date = DateTimeOffset.Now;
                ViewModels.currentId = -1;
                Windows.UI.Xaml.Media.Imaging.BitmapImage bit = new Windows.UI.Xaml.Media.Imaging.BitmapImage();
                bit.UriSource = new Uri(Img.BaseUri, "../Assets/icon.jpg");
                Img.Source = bit;
                CreateBut.Content = "Create";
            }
        }

        private void Share_Click(object sender, RoutedEventArgs e)
        {
            this.ViewModels.currentId = ((sender as MenuFlyoutItem).DataContext as Models.Item).id;
            DataTransferManager dataTransferManager = DataTransferManager.GetForCurrentView();
            dataTransferManager.DataRequested += DataTransferManager_Requested;
            DataTransferManager.ShowShareUI();
        }
        public async void DataTransferManager_Requested(DataTransferManager sender, DataRequestedEventArgs args)
        {
            var dp = args.Request.Data;
            var deferral = args.Request.GetDeferral();
            var image = await StorageFile.GetFileFromApplicationUriAsync(new Uri("ms-appx:///Assets/Forest.jpg"));
            dp.Properties.Title = this.ViewModels.Items[this.ViewModels.currentId].topic;
            dp.Properties.Description = this.ViewModels.Items[this.ViewModels.currentId].content;
            dp.SetStorageItems(new List<StorageFile> { image });
            dp.SetText(this.ViewModels.Items[this.ViewModels.currentId].content + "\nDueDate:" + this.ViewModels.Items[this.ViewModels.currentId].date.ToString());
            deferral.Complete();
        }
        private void Add_Click(object sender, RoutedEventArgs e)
        {
            //跳转或者在右侧栏显示
            ViewModels.currentId = -1;
            if (InternalEdit.Visibility == Visibility.Visible)
            {
                Title.Text = "";
                Detail.Text = "";
                Date.Date = DateTimeOffset.Now;
                Windows.UI.Xaml.Media.Imaging.BitmapImage bit = new Windows.UI.Xaml.Media.Imaging.BitmapImage();
                bit.UriSource = new Uri(Img.BaseUri, "../Assets/icon.jpg");
                Img.Source = bit;
                Title.Focus(FocusState.Pointer);
                CreateBut.Content = "Create";
                return;
            }
            var item = ListView.SelectedItem as Models.Item;
            this.Frame.Navigate(typeof(NewPage), " ");
        }
        private async void Select_Click(object sender, RoutedEventArgs e)
        {   
            //选择图片
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
            //创建model并修改vm
            var dialog = new ContentDialog()
            {
                Title = "提示",
                PrimaryButtonText = "确定",
            };
            if (Title.Text == "" || Detail.Text == "") dialog.Content += "Title栏和Detail栏不可为空！\n";
            DateTime now = new DateTime(DateTime.Now.Year, DateTime.Now.Month, DateTime.Now.Day);
            if (Date.Date.CompareTo(now) < 0) dialog.Content += "DueDate不可设为过去的日期！\n";
            if (dialog.Content != null)
            {
                await dialog.ShowAsync();
                return;
            }
            if (ViewModels.currentId == -1) ViewModels.AddItem(Title.Text, Detail.Text, Date.Date, Img.Source);
            else ViewModels.ChangeItem(ViewModels.currentId, Title.Text, Detail.Text, Date.Date, Img.Source);
            ViewModels.currentId = -1;
            Title.Text = "";
            Detail.Text = "";
            Date.Date = DateTime.Now;
            CreateBut.Content = "Create";
            Windows.UI.Xaml.Media.Imaging.BitmapImage bit = new Windows.UI.Xaml.Media.Imaging.BitmapImage();
            bit.UriSource = new Uri(Img.BaseUri, "../Assets/icon.jpg");
            Img.Source = bit;
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
        }
        private void CancelClick(Object sender, RoutedEventArgs e)
        {   
            Title.Text = "";
            Detail.Text = "";
            Date.Date = DateTime.Now;
            ViewModels.currentId = -1;
            ListView.SelectedIndex = -1;
            CreateBut.Content = "Create";
            Windows.UI.Xaml.Media.Imaging.BitmapImage bit = new Windows.UI.Xaml.Media.Imaging.BitmapImage();
            bit.UriSource = new Uri(Img.BaseUri, "../Assets/icon.jpg");
            Img.Source = bit;
        }
        private void Bottom_Delete_Click(Object s, RoutedEventArgs e)
        {
            ViewModels.RemoveItem(ViewModels.currentId);
            Title.Text = "";
            Detail.Text = "";
            Date.Date = DateTimeOffset.Now;
            Windows.UI.Xaml.Media.Imaging.BitmapImage bit = new Windows.UI.Xaml.Media.Imaging.BitmapImage();
            bit.UriSource = new Uri(Img.BaseUri, "../Assets/icon.jpg");
            Img.Source = bit;
            CreateBut.Content = "Create";
            ViewModels.currentId = -1;
            this.DataContext = ViewModels;
        }
    }
}
