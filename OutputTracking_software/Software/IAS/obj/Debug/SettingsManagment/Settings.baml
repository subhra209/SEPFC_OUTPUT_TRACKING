   M S B A M L   `   `   `  ����O  Kmscorlib, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e0891&clr-namespace:System;assembly=mscorlibSystem  5      > :IAS, Version=1.0.0.0, Culture=neutral, PublicKeyToken=nullclr-namespace:IASIAS 5         IAS.Settings   5      \ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35U QWPFToolkit, Version=3.5.40128.1, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35F 9http://schemas.microsoft.com/winfx/2006/xaml/presentation     R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5      6sys&clr-namespace:System;assembly=mscorlib     5      6dg-http://schemas.microsoft.com/wpf/2008/toolkit  5      localclr-namespace:IAS  5        N� Title$  Settings��5	       N� 
Background
 � � � � 	Resources 5       �� 
TargetType �� Property �� Value��%�   '��       '�� �     �� 5   
   " ��6   .�5      ��  � ��$ Right=�64   �� 5       � ��
 �����6+   �� 5       � ��$ 20(�6)   5      �� 5   
   " ��6   .�5      ��  � ��$ 20(�6)   5      5      .��5      � -   5        $��settingsGrid��5   	    � RowDefinitions 5   
   �� 5      �� 5      �� 5      �� 5       $
��Auto �6   .+�5"   
   �� $	��Top=�6   ��5#      �� 5$      $��Marquee��6   
�������$��12(�6=   .��5&      �� .+�5'      ��  �� Orientation$ 
Horizontal=�6   $��5q�66   .+�5(      �� $�� Message Marquee:��6   �� 5)      -   5        $��	tbMarquee��5)      $	��500��6/   ��6>   ��  �� Path$ Marquee�	 �� Mode$	 TwoWay=�5*      �� 5+      $��5q�6   $ 
Horizontal=�6(   .+�5,      �� $��Message Marquee Speed:��6   �� 5-      -   5        $��tbMarqueeSpeed��5-      $��50��60   ��6>   �� $ MarqueeSpeed�$	 TwoWay=�5.      �� 5/      $��5q�6   $ 
Horizontal=�6(   $y�Visible=�6A   .+�50      �� $��Issue Marquee Speed:��6   �� 51      -   5        $��tbIssueMarqueeSpeed��51      $��50��69   ��6G   �� $ IssuemarqueeSpeed�$	 TwoWay=�53      55      57      �� 58   
   $��1��6   ��59      �� 5:      $��Escalation Settings��6   
�������$��12(�6I   .��5<   
   � $��5q�6   $y�Visible=�6   $
��Auto��60   
 � ColumnDefinitions
 5=      �� 5>      $
��Auto �6#   �� 5?      $
��Auto �6#   .+�5A      )  #Microsoft.Windows.Controls.DataGrid  -   5        $��escalationDurationTable��5A   8      AutoGenerateColumns .  $
��Left=�5B   ;      SelectionMode$ Single=�5C   "      SelectionUnit$ FullRow=�69   
��� � �$��0q�5D   8      CanUserAddRows .     CanUserReorderColumns .     CanUserSortColumns .  ��5B   "   ��  5G      @  :Microsoft.Windows.Controls.Primitives.DataGridColumnHeader��%�   '        �� 5H      Header_Style1��"  63   .�5I      ��  � ��$ 15(�65   �� 5J       � ��$
 Bold"�67   �� 5K       � ��$ Center=�6@   �� 5L       � ��$ 0q�6<   5N      5O         Columns 5P      7  1Microsoft.Windows.Controls.DataGridTemplateColumn  5Q      /  )Microsoft.Windows.Controls.DataGridColumn   Header$ 
Escalation��60      
IsReadOnly .    CanUserReorder .     CellTemplate 5R      �� 5S      .R�5T   "   �� $��15(�6E   ��6-   �� *Name+6E   5U      5W        5X      $ Duration(in Minutes)��60    5Y      �� 5Z      .R�5[   "   ��  �� MaxLines$ 1��6*    �� 	MaxLength$ 8��67   $��20(�5]   6   ��5\   6   �� *
Duration+$	 TwoWay=�5]   6   5^      5`      5b      5d      5e      �� 5f   
   $��2��6   $
��Auto��6!   ��5g      �� 5h      $��Password��6   
�������$��12(�6>   .��5j      �� $ 
Horizontal=�6   $��5q�64   $
��Auto��6?   .+�5k      �� $�� Current Password:��6   G� 5l      -   5        $��tbOldPassword��5l   "   $	��100��67    G� PasswordChar$ *��6F   $	��Top=�5m   "    G� 	MaxLength$ 4��6:   $��20(�6H   $
��Auto��6V   5n   "   �� 5o      $��New Password:��6   $��5,0,0,0q�64   G� 5p      -   5        $��tbnewPassword��5p   "   $	��100��67   $ *��6F   $	��Top=�5q   "   $ 4��6:   $��20(�6H   $
��Auto��6V   {�.  5r   5   5u      5|      �� 5}   
   -   5        $��btnDone��5}      $��3��6   $��Right=�6-   $��Bottom=�6I   $
��DONE��5~      $��20(�6    $��5q�6.   69   5�      5�      