   M S B A M L   `   `   `  ����  IAS     IAS.addModifyDeleteControl   5      \ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35U QWPFToolkit, Version=3.5.40128.1, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35F 9http://schemas.microsoft.com/winfx/2006/xaml/presentation     5      R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5      >
wpfToolKit-http://schemas.microsoft.com/wpf/2008/toolkit  5      $
��Auto��5      $
��Auto��6     � 	Resources  5      )  #Microsoft.Windows.Controls.DataGrid �� 
TargetType �� Property �� Value��%b  '        '�� J     �� 5   
   "  6   .�5      ��  � ��
 � � �6+   5      �� 5   
   " ��6   .�5      ��  � ��$ 20(�6)   �� 5       � ��$ 10q�6'   �� 5       � ��$ 50��6)   �� 5       � ��$
 Auto��6&   �� 5       � ��$
 Auto��6'   �� 5       � ��$ Center=�64   5      5      .��5      �� -   5        $��aMDGroupBox��5      
��� � �
�������$��1q�5   #   $
��Auto��67   ��5   
   �� 5      
���������6   ��  �� RelativeSource �� *TemplatedParent+ �� Path$ Header�5       .��5$   
   �  � RowDefinitions 5%      �� 5&      $��* �6    �� 5'      $
��Auto �6!   .+�5)      �� $��1��6    �� Orientation$ 
Horizontal=�6&   $
��Left=�6@   $��Bottom=�6[   .+�5*      �� -   5        $��btnAdd��5*      $	��Add��6'   $��Bottom=�6J   $��5q�6e   �� 5,      -   5        $��	btnModify��5,      $��Modify��6*   $��Bottom=�6S   $��5q�6n   $y�	Collapsed=�6y   �� 5-      -   5        $��	btnDelete��5-      $��Delete��6*   $��Bottom=�6S   $��5q�6n   50        52      -   5        $��dgItem��52   "      AutoGenerateColumns .  
��� � �	   
IsReadOnly	 . 
   SelectionMode$
 Single=�53   4      SelectionUnit$ FullRow=�6K      HeadersVisibility$ Column=�54   "   $��0q�6>   $
��Left=�55   "   $��Stretch=�6=   "   VerticalScrollBarVisibility$
 Auto=�57   !   $   HorizontalScrollBarVisibility$
 Auto=�6D   $��	0,10,0,31q�6i   $��2��6|   ��52   0   ��    Columns 58      3  -Microsoft.Windows.Controls.DataGridTextColumn  59      /  )Microsoft.Windows.Controls.DataGridColumn   Header$
 Name��5:   5      
Foreground
 �   �   FontSize$ 15(�6V   4  .Microsoft.Windows.Controls.DataGridBoundColumn   Binding 59   5   �� $
 Name�5:   V   7  1Microsoft.Windows.Controls.DataGridTemplateColumn  5<      $ 	CycleTime��69      CellTemplate 5=      �� 5>      .R�5?   "   �� $��Stretch=�6,   $��15(�6m   $��50��5@   ,   ��5?   K   �� $ 	CycleTime�5@   ,   5A      5D        5F      $ BottleNeckTime��69    5G      �� 5H      .R�5I   "   �� $��Stretch=�6,   $��15(�6r   $��50��5J   ,   ��5I   K   �� $ BottleNeckTime�5J   ,   5K      5N      5S      5V      5Y      5Z      