   M S B A M L   `   `   `  ����O  Kmscorlib, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e0891&clr-namespace:System;assembly=mscorlibSystem  5      > :IAS, Version=1.0.0.0, Culture=neutral, PublicKeyToken=nullclr-namespace:IASIAS 5         IAS.LineManagement   5      \ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35U QWPFToolkit, Version=3.5.40128.1, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35F 9http://schemas.microsoft.com/winfx/2006/xaml/presentation     R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5      6sys&clr-namespace:System;assembly=mscorlib     5      6dg-http://schemas.microsoft.com/wpf/2008/toolkit  5      localclr-namespace:IAS  5        N� Title$  LineManagement��5	       N� 
Background
 � � �$
��Auto��5
      $
��Auto��6&    N� ShowsNavigationUI .   � 	Resources 5      )  #Microsoft.Windows.Controls.DataGrid �� 
TargetType �� Property �� Value��%T   '        �� 5   
   "  6   .�5      ��  � ��
 � � �6+   5      5      .��5      � $
��Auto��6   $
��Auto��6    � RowDefinitions 5   
   �� 5      $��* �6   �� 5      $
��Auto �6   .+�5   
   �� -   5        $��tbcLineControl��5      
��� � �.@�5      �� $��LINES��6   .��5      A�  A� Columns$ 2��6   .+�5        IAS.DisplayControl  -   5        	  Name$	 lineControl��5   ;     IAS.DisplayControl
   selectionChanged$"
 lineControl_selectionChanged��5   +   $��0��5   +   $��5q�6P   5"         IAS.addModifyDeleteControl  5%      -   5          Name$ stationControl��5%   C      IAS.addModifyDeleteControl   selectionChanged$% stationControl_selectionChanged��5&   +      
addClicked$ stationControl_addClicked��5'   +      modifyClicked$" stationControl_modifyClicked��5(   +      deleteClicked$" stationControl_deleteClicked��5)   +   $��1��5%   3   $��0,5,5,5q�6[   $
��Auto��5+   +   $
��Auto��68   5-      5D      5E      5G      �� 5H   
   -   5        $��1��5H      $��Right=�6   $��Bottom=�6:   $
��DONE��6U   $��5q�6d   $��20(�5I   '   5K      5L      