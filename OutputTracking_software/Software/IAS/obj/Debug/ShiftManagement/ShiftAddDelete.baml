   M S B A M L   `   `   `  ����  IAS    IAS.ShiftAddDelete   5      \ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35U QWPFToolkit, Version=3.5.40128.1, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35F 9http://schemas.microsoft.com/winfx/2006/xaml/presentation     5      R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5      >
wpfToolKit-http://schemas.microsoft.com/wpf/2008/toolkit  5      $
��Auto��5      $
��Auto��6     � 	Resources  5      )  #Microsoft.Windows.Controls.DataGrid �� 
TargetType �� Property �� Value��%�   '        '�� J     �� 5      "  6   .�5      ��  � ��
 � � �6/   5
      �� 5      " ��6   .�5      ��  � ��$ 20(�6-   �� 5       � ��$ 10q�6+   �� 5       � ��$ 50��6-   5      5      .��5   
   �� -   5        $��aMDGroupBox��5      
��� � �$��10q�69   
�������$��1q�5   #   ��5   
   �� 5      $��Header��6   
�������5      .��5      �  � RowDefinitions 5      �� 5      $��* �6    �� 5      $
��Auto �6    .+�5        -   5        $��dgItem��5   3   $��0��6&      AutoGenerateColumns .  
��� � �   
IsReadOnly .    SelectionMode$ Single=�5   4      SelectionUnit$ FullRow=�6K   	   HeadersVisibility$	 Column=�5    "   $
��Auto��6=   $
��Auto��5!   "   $��Center=�6/   $��Center=�6L   $��0q�5#   !   ��5   A   �� 
   Columns
 5$      3  -Microsoft.Windows.Controls.DataGridTextColumn  5&      /  )Microsoft.Windows.Controls.DataGridColumn   Header$
 NAME��6V      
Foreground
 �   �   FontSize$ 15(�6w   4  .Microsoft.Windows.Controls.DataGridBoundColumn   Binding 68   ��  �� Path$
 Name�6w     5'      $ 
START TIME��6[   
 �   �$ 15(�6�    68   �� $ 	StartTime�6�     5(      $ END TIME��6Y   
 �   �$ 15(�6~    68   �� $ EndTime�6~   50      �� 52       �� Orientation$ 
Horizontal=�6   $��1��67   $��Bottom=�6D   $
��Left=�53      .+�54      �� -   5        $��btnAdd��54      $��+��6+   $��Bottom=�6L   $
��Left=�55      �� 57      -   5        $��	btnDelete��57      $��-��6.   $��Bottom=�6R   $
��Left=�58      5:      5;      5>      5A      