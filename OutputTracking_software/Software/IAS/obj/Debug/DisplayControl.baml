   M S B A M L   `   `   `  ����  IAS    IAS.DisplayControl   5      \ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35U QWPFToolkit, Version=3.5.40128.1, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35F 9http://schemas.microsoft.com/winfx/2006/xaml/presentation     5      R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5      Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     5      @d2http://schemas.microsoft.com/expression/blend/2008     5      >
wpfToolKit-http://schemas.microsoft.com/wpf/2008/toolkit  5      $
��Auto��5      $
��Auto��6     � 	Resources  5      )  #Microsoft.Windows.Controls.DataGrid �� 
TargetType �� Property �� Value��%b  '        '�� J     �� 5   
   "  6   .�5      ��  � ��
 � � �6+   5      �� 5   
   " ��6   .�5      ��  � ��$ 20(�6)   �� 5       � ��$ 10q�6'   �� 5       � ��$ 50��6)   �� 5       � ��$
 Auto��6&   �� 5       � ��$
 Auto��6'   �� 5       � ��$ Center=�64   5      5      .��5      �� -   5        $��aMDGroupBox��5      
��� � �
�������$��1q�5   #   $
��Auto��67   ��5   
   �� 5!      
���������6   ��  �� RelativeSource �� *TemplatedParent+ �� Path$ Header�5"      .��5&   
   �  � RowDefinitions 5'      �� 5(      $��* �6    �� 5)      $
��Auto �6!   .+�54        -   5        $��dgItem��54   "      AutoGenerateColumns .  
��� � �   
IsReadOnly . 	   SelectionMode$	 Single=�55   4   
   SelectionUnit$
 FullRow=�6K      HeadersVisibility$ Column=�56   "   $��0q�6>   $
��Left=�57   "   $��Stretch=�6=   "   VerticalScrollBarVisibility$
 Auto=�59   !   $   HorizontalScrollBarVisibility$
 Auto=�6D   $��	0,10,0,31q�6i   $��2��6|   ��54   0   ��    Columns 5:      3  -Microsoft.Windows.Controls.DataGridTextColumn  5;      /  )Microsoft.Windows.Controls.DataGridColumn   Header$
 NAME��5<   5      
Foreground
 �   �   FontSize$ 15(�6V   4  .Microsoft.Windows.Controls.DataGridBoundColumn   Binding 5;   5   �� $
 Name�5<   V   5D      5G      5J      5K      