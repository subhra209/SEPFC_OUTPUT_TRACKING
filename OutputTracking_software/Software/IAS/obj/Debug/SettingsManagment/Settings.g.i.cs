﻿#pragma checksum "..\..\..\SettingsManagment\Settings.xaml" "{406ea660-64cf-4c82-b6f0-42d48172a799}" "FAEB46B2B02F7A21F52555997D2D0F0A"
//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime Version:4.0.30319.1022
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

using IAS;
using Microsoft.Windows.Controls;
using Microsoft.Windows.Controls.Primitives;
using System;
using System.Diagnostics;
using System.Windows;
using System.Windows.Automation;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Markup;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Effects;
using System.Windows.Media.Imaging;
using System.Windows.Media.Media3D;
using System.Windows.Media.TextFormatting;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Shell;


namespace IAS {
    
    
    /// <summary>
    /// Settings
    /// </summary>
    [System.CodeDom.Compiler.GeneratedCodeAttribute("PresentationBuildTasks", "4.0.0.0")]
    public partial class Settings : System.Windows.Navigation.PageFunction<IAS.settings>, System.Windows.Markup.IComponentConnector {
        
        
        #line 27 "..\..\..\SettingsManagment\Settings.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Grid settingsGrid;
        
        #line default
        #line hidden
        
        
        #line 41 "..\..\..\SettingsManagment\Settings.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.TextBox tbMarquee;
        
        #line default
        #line hidden
        
        
        #line 45 "..\..\..\SettingsManagment\Settings.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.TextBox tbMarqueeSpeed;
        
        #line default
        #line hidden
        
        
        #line 49 "..\..\..\SettingsManagment\Settings.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.TextBox tbIssueMarqueeSpeed;
        
        #line default
        #line hidden
        
        
        #line 65 "..\..\..\SettingsManagment\Settings.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal Microsoft.Windows.Controls.DataGrid escalationDurationTable;
        
        #line default
        #line hidden
        
        
        #line 108 "..\..\..\SettingsManagment\Settings.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.PasswordBox tbOldPassword;
        
        #line default
        #line hidden
        
        
        #line 112 "..\..\..\SettingsManagment\Settings.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.PasswordBox tbnewPassword;
        
        #line default
        #line hidden
        
        
        #line 125 "..\..\..\SettingsManagment\Settings.xaml"
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        internal System.Windows.Controls.Button btnDone;
        
        #line default
        #line hidden
        
        private bool _contentLoaded;
        
        /// <summary>
        /// InitializeComponent
        /// </summary>
        [System.Diagnostics.DebuggerNonUserCodeAttribute()]
        public void InitializeComponent() {
            if (_contentLoaded) {
                return;
            }
            _contentLoaded = true;
            System.Uri resourceLocater = new System.Uri("/IAS;component/settingsmanagment/settings.xaml", System.UriKind.Relative);
            
            #line 1 "..\..\..\SettingsManagment\Settings.xaml"
            System.Windows.Application.LoadComponent(this, resourceLocater);
            
            #line default
            #line hidden
        }
        
        [System.Diagnostics.DebuggerNonUserCodeAttribute()]
        [System.ComponentModel.EditorBrowsableAttribute(System.ComponentModel.EditorBrowsableState.Never)]
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Design", "CA1033:InterfaceMethodsShouldBeCallableByChildTypes")]
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Maintainability", "CA1502:AvoidExcessiveComplexity")]
        [System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1800:DoNotCastUnnecessarily")]
        void System.Windows.Markup.IComponentConnector.Connect(int connectionId, object target) {
            switch (connectionId)
            {
            case 1:
            this.settingsGrid = ((System.Windows.Controls.Grid)(target));
            return;
            case 2:
            this.tbMarquee = ((System.Windows.Controls.TextBox)(target));
            return;
            case 3:
            this.tbMarqueeSpeed = ((System.Windows.Controls.TextBox)(target));
            return;
            case 4:
            this.tbIssueMarqueeSpeed = ((System.Windows.Controls.TextBox)(target));
            return;
            case 5:
            this.escalationDurationTable = ((Microsoft.Windows.Controls.DataGrid)(target));
            return;
            case 6:
            this.tbOldPassword = ((System.Windows.Controls.PasswordBox)(target));
            
            #line 110 "..\..\..\SettingsManagment\Settings.xaml"
            this.tbOldPassword.PasswordChanged += new System.Windows.RoutedEventHandler(this.tbOldPassword_PasswordChanged);
            
            #line default
            #line hidden
            return;
            case 7:
            this.tbnewPassword = ((System.Windows.Controls.PasswordBox)(target));
            
            #line 114 "..\..\..\SettingsManagment\Settings.xaml"
            this.tbnewPassword.LostFocus += new System.Windows.RoutedEventHandler(this.tbnewPassword_LostFocus);
            
            #line default
            #line hidden
            return;
            case 8:
            this.btnDone = ((System.Windows.Controls.Button)(target));
            
            #line 126 "..\..\..\SettingsManagment\Settings.xaml"
            this.btnDone.Click += new System.Windows.RoutedEventHandler(this.btnDone_Click);
            
            #line default
            #line hidden
            return;
            }
            this._contentLoaded = true;
        }
    }
}

