   M S B A M L   `   `   `  ����  IAS    IAS.ContactAddDelete   5      \ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35U QWPFToolkit, Version=3.5.40128.1, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35F 9http://schemas.microsoft.com/winfx/2006/xaml/presentation     5      R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5      >
wpfToolKit-http://schemas.microsoft.com/wpf/2008/toolkit  5      $
��Auto��5      $
��Auto��6     � 	Resources  5      )  #Microsoft.Windows.Controls.DataGrid �� 
TargetType �� Property �� Value��%�   '        '�� J     �� 5   
   "  6   .�5      ��  � ��
 � � �6+   5      �� 5   
   " ��6   .�5      ��  � ��$ 20(�6)   �� 5       � ��$ 10q�6'   �� 5       � ��$ 50��6)   5      5      .��5      �� -   5        $��aMDGroupBox��5      
��� � �
�������$��2q�5   #   ��5   
   �� 5      
�������6   .��5   
   ]� .+�5      �� $��Bottom=�6    �� Orientation$ 
Horizontal=�61   .+�5      �� -   5        $��btnAdd��5      $��+��6'   $��Bottom=�6H   $��5q�6c   �� 5!      -   5        $��	btnDelete��5!      $��-��6*   $��Bottom=�6N   $��5q�6i   5#        5%      -   5        $��dgItem��5%   "      AutoGenerateColumns .  
��� � �   
IsReadOnly .    SelectionMode$ Single=�5&   4      SelectionUnit$ FullRow=�6K   	   HeadersVisibility$	 Column=�5'   "   $��Center=�5(   "   $
��Left=�6?   $
   HorizontalScrollBarVisibility$

 Auto=�6a   "   VerticalScrollBarVisibility$
 Auto=�6�   $��0q�5)   M      CanUserAddRows .     CanUserDeleteRows .  ��5%   0   ��    Columns 5+      3  -Microsoft.Windows.Controls.DataGridTextColumn  5.      /  )Microsoft.Windows.Controls.DataGridColumn   Header$
 NAME��6R      
Foreground
 �   �   FontSize$ 15(�6s   4  .Microsoft.Windows.Controls.DataGridBoundColumn   Binding 64   ��  �� Path$
 Name�5/      53      54      57      58      