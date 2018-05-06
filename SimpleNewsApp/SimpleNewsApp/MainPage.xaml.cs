using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net.Http;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Data.Json;
using Windows.Data.Xml.Dom;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;
using Windows.UI.Xaml.Shapes;

// https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x804 上介绍了“空白页”项模板

namespace SimpleNewsApp
{
    /// <summary>
    /// 可用于自身或导航至 Frame 内部的空白页。
    /// </summary>
    public sealed partial class MainPage : Page
    {
        public MainPage()
        {
            this.InitializeComponent();
        }

        private async void search_Click(object sender, RoutedEventArgs e)
        {
            string keyword = inputword.Text;
            string appkey = "7926b545c1629cbc";
            string url = "http://api.jisuapi.com/news/search?keyword=" + keyword + "&appkey=" + appkey;
            HttpClient client = new HttpClient();
            HttpResponseMessage res = await client.GetAsync(url);
            res.EnsureSuccessStatusCode();
            string resStr = await res.Content.ReadAsStringAsync();
            JsonObject jsobj = JsonValue.Parse(resStr).GetObject();
            if(jsobj["status"].GetString() != "0")
            {
                return;
            }
            JsonObject result = jsobj["result"].GetObject();
            JsonArray resarr = result["list"].GetArray();
            for (uint i = 0; i < uint.Parse(result["num"].GetString()); i++)
            {
                JsonObject news = resarr.GetObjectAt(i);
                TextBlock title = new TextBlock();
                title.Width = 800;
                title.HorizontalAlignment = HorizontalAlignment.Left;
                title.TextAlignment = TextAlignment.Center;
                title.FontSize = 22;
                title.Text = news["title"].GetString();
                TextBlock author = new TextBlock();
                title.Text = news["title"].GetString();
                author.Width = 800;
                author.HorizontalAlignment = HorizontalAlignment.Left;
                author.TextAlignment = TextAlignment.Center;
                author.FontSize = 14;
                author.Text = news["src"].GetString();
                WebView content = new WebView();
                content.Width = 800;
                content.Height = 550;
                content.HorizontalAlignment = HorizontalAlignment.Left;
                content.NavigateToString(news["content"].GetString());
                Line line = new Line();
                line.Stroke = new SolidColorBrush(Colors.Black);
                line.StrokeThickness = 1;
                line.Margin = new Thickness(0, 30, 0, 30);
                line.X2 = 800;
                panel.Children.Add(title);
                panel.Children.Add(author);
                panel.Children.Add(content);
                panel.Children.Add(line);
            }
        }
    }
}
