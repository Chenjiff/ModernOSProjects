﻿#pragma checksum "C:\Users\chenjifff\Desktop\app1\App1\App1\Views\NewPage.xaml" "{406ea660-64cf-4c82-b6f0-42d48172a799}" "5F0648FDF8A7D0171E03CAD1C19B63E6"
//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

namespace App1.Views
{
    partial class NewPage : 
        global::Windows.UI.Xaml.Controls.Page, 
        global::Windows.UI.Xaml.Markup.IComponentConnector,
        global::Windows.UI.Xaml.Markup.IComponentConnector2
    {
        /// <summary>
        /// Connect()
        /// </summary>
        [global::System.CodeDom.Compiler.GeneratedCodeAttribute("Microsoft.Windows.UI.Xaml.Build.Tasks"," 14.0.0.0")]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        public void Connect(int connectionId, object target)
        {
            switch(connectionId)
            {
            case 1:
                {
                    global::Windows.UI.Xaml.Controls.Button element1 = (global::Windows.UI.Xaml.Controls.Button)(target);
                    #line 20 "..\..\..\Views\NewPage.xaml"
                    ((global::Windows.UI.Xaml.Controls.Button)element1).Click += this.Select_Click;
                    #line default
                }
                break;
            case 2:
                {
                    this.Title = (global::Windows.UI.Xaml.Controls.TextBox)(target);
                }
                break;
            case 3:
                {
                    this.Detail = (global::Windows.UI.Xaml.Controls.TextBox)(target);
                }
                break;
            case 4:
                {
                    this.Date = (global::Windows.UI.Xaml.Controls.DatePicker)(target);
                }
                break;
            case 5:
                {
                    this.CreateBut = (global::Windows.UI.Xaml.Controls.Button)(target);
                    #line 28 "..\..\..\Views\NewPage.xaml"
                    ((global::Windows.UI.Xaml.Controls.Button)this.CreateBut).Click += this.CreateClick;
                    #line default
                }
                break;
            case 6:
                {
                    this.CancelBut = (global::Windows.UI.Xaml.Controls.Button)(target);
                    #line 29 "..\..\..\Views\NewPage.xaml"
                    ((global::Windows.UI.Xaml.Controls.Button)this.CancelBut).Click += this.CancelClick;
                    #line default
                }
                break;
            case 7:
                {
                    this.Img = (global::Windows.UI.Xaml.Controls.Image)(target);
                }
                break;
            case 8:
                {
                    global::Windows.UI.Xaml.Controls.AppBarButton element8 = (global::Windows.UI.Xaml.Controls.AppBarButton)(target);
                    #line 36 "..\..\..\Views\NewPage.xaml"
                    ((global::Windows.UI.Xaml.Controls.AppBarButton)element8).Click += this.Delete_Click;
                    #line default
                }
                break;
            default:
                break;
            }
            this._contentLoaded = true;
        }

        [global::System.CodeDom.Compiler.GeneratedCodeAttribute("Microsoft.Windows.UI.Xaml.Build.Tasks"," 14.0.0.0")]
        [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
        public global::Windows.UI.Xaml.Markup.IComponentConnector GetBindingConnector(int connectionId, object target)
        {
            global::Windows.UI.Xaml.Markup.IComponentConnector returnValue = null;
            return returnValue;
        }
    }
}
