   M S B A M L   `   `   `  ����O  Kmscorlib, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e0891&clr-namespace:System;assembly=mscorlibSystem  5      > :IAS, Version=1.0.0.0, Culture=neutral, PublicKeyToken=nullclr-namespace:IASIAS 5          IAS.SupportGroupManagement   5      \ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35U QWPFToolkit, Version=3.5.40128.1, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35F 9http://schemas.microsoft.com/winfx/2006/xaml/presentation     R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5      6sys&clr-namespace:System;assembly=mscorlib     5      #localclr-namespace:IAS     5      -   5          N� Title$  SupportGroupManagement��5       N� 
Background
 � � � � 	Resources 5      ��%    5      .��5      �  � RowDefinitions 5   
   �� 5      $��* �6   �� 5      $
��AUTO �6   .+�5   
   ]� .+�5        IAS.ContactAddDelete  -   5          Name$ contactControl��5   %     IAS.ContactAddDelete   selectionChanged$% contactControl_selectionChanged��5   /      
addClicked$ contactControl_addClicked��5   /      deleteClicked$" contactControl_deleteClicked��5   /      Header$ CONTACTS��5   %   $
��Left=�67   5      �� 5      -   5        $��ContactDetails��5      
�������$��2q�6B   $��50,0,0,0q�6W   $y�Hidden=�6i   ��5      �� 5!      $��CONTACT DETAILS��6   
�������$��12(�6I   .��5$      ]� .+�5&      �� -   5        $��btnSave��5&   5   $��Bottom=�6   $
��SAVE��6D   $��5q�6S   $
��Auto��6^   $
��Auto��6k   $��Right=�5'      $��20(�5(      6$     IAS.ContactDetails  5*      -   5        	  Name$	 contactDetailsControl��5*   +   $��10,0,0,0q�6J   5-      52      54      55      �� 58   
   -   5        $��1��58      $��Right=�6   $��Bottom=�6:   $
��DONE��6U   $��5q�6d   $��20(�59      6   5=      5>      