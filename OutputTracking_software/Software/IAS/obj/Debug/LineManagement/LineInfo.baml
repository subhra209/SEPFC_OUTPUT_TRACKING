   M S B A M L   `   `   `  ����O  Kmscorlib, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e0891&clr-namespace:System;assembly=mscorlibSystem  5      > :IAS, Version=1.0.0.0, Culture=neutral, PublicKeyToken=nullclr-namespace:IASIAS 5         IAS.LineInfo   5      \ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35U QWPFToolkit, Version=3.5.40128.1, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35F 9http://schemas.microsoft.com/winfx/2006/xaml/presentation     R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5      6sys&clr-namespace:System;assembly=mscorlib     5      #localclr-namespace:IAS     5        N� Title$  LineInfo��5	      $
��Auto��6   $
��Auto��6$    � 	Resources 5
       �� 
TargetType �� Property �� Value��%�  '��       '�� m     '�� '    �� 5   
   " ��6   .�5      ��  � ��$	 300��6)   �� 5       � ��$ 20(�6)   5      �� 5   
   " ��6   .�5      ��  � ��$	 100��6)   �� 5       � ��$ Right=�64   �� 5       � ��
 �����6+   �� 5       � ��$ 20(�6)   5      �� 5   
   " ��6   .�5      ��  � ��$ 30(�6)   5      5      .��5      �� 
��� � �.+�5      � -   5        $��IdGrid��5      $
��5,20q�6   $
��Auto��6(    � ColumnDefinitions 5    
   �� 5!      $
��Auto �6   �� 5"      $
��Auto �6   .+�5$   
   �� $��0��6   $��	 LINE ID:��6!   $
��Left=�65   �� 5%      -   5        $��tbLineID��5%   &   $��1��6    �� 
IsReadOnly .  ��67   ��  �� Path$ ID� �� Mode$ TwoWay=�	 �� UpdateSourceTrigger$	 PropertyChanged=�5&      5(      � 5)   
   -   5        $��NameGrid��5)      $
��5,20q�6   $
��Auto��6-    5*      �� 5+      $
��Auto �6#   �� 5,      $
��Auto �6#   .+�5.      �� $��0��6   $�� LINE NAME:��6%   $
��Left=�6;   �� 5/      -   5        $��
tbLineName��5/   &   $��1��6    .  ��69   �� $
 Name�$ TwoWay=�$	 PropertyChanged=�50      52      �� 53   
   
 �� Orientation$
 
Horizontal=�6   $��Center=�6.   .+�54      �� -   5        $��btnSave��54      $
��Save��6$   $��5q�63   6>   �� 55      -   5        $��	btnCancel��55      $��Cancel��6&   $��5q�67   6B   57      59      5;      