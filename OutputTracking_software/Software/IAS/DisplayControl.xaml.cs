using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace IAS
{
    /// <summary>
    /// Interaction logic for DisplayControl.xaml
    /// </summary>
    public partial class DisplayControl : UserControl
    {
        public event EventHandler<AddModifyDeleteSelectionChangedEventArgs> selectionChanged;
        public DisplayControl()
        {
            InitializeComponent();
        }

        private void dgItem_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            AddModifyDeleteSelectionChangedEventArgs eventArgs;

            eventArgs = new AddModifyDeleteSelectionChangedEventArgs(dgItem.SelectedIndex);

            if (selectionChanged != null)
                selectionChanged(this, eventArgs);
        }

    }
}
