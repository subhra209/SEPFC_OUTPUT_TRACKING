   M S B A M L   `   `   `  ����>  :IAS, Version=1.0.0.0, Culture=neutral, PublicKeyToken=nullclr-namespace:IASIAS  5          IAS.ContactDetails   5      \ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35U QWPFToolkit, Version=3.5.40128.1, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35F 9http://schemas.microsoft.com/winfx/2006/xaml/presentation     5      R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5      6dg-http://schemas.microsoft.com/wpf/2008/toolkit  5      localclr-namespace:IAS  5      $
��Auto��5      $
��Auto��6     � 	Resources  5       �� 
TargetType �� Property �� Value��%�  '��       '�� m     '�� '    �� 5	   
   " ��6   .�5
      ��  � ��$	 300��6)   �� 5       � ��$ 20(�6)   5      �� 5   
   " ��6   .�5      ��  � ��$	 100��6)   �� 5       � ��$ Right=�64   �� 5       � ��
 �����6+   �� 5       � ��$ 20(�6)   5      �� 5   
   " ��6   .�5      ��  � ��$ 30(�6)   5      5      .��5      � 
��� � � � RowDefinitions 5   
   �� 5       $
��Auto �6   �� 5!      $
��Auto �6   �� 5"      $
��Auto �6   �� 5#      $
��Auto �6   .+�5&      � -   5        $��DeviceIdGrid��5&       $��0��6   $	��5,5q�65   $
��Auto��6B    � ColumnDefinitions 5'      �� 5(      $
��Auto �6'   �� 5)      $
��Auto �6'   .+�5+      �� $��0��6   $�� CONTACT NAME:��6)   $
��Left=�6B   �� 5,      -   5        $��tbContactName��5,   *   $��1��6    �� 
IsReadOnly .  ��6A   ��  �� Path$
 Name�5-      5/      � 50      $��1��6   $	��5,5q�6    $
��Auto��6-    51      �� 52      $
��Auto �6'   �� 53      $
��Auto �6'   .+�55      �� $��0��6   $�� CONTACT NUMBER:��6)   $
��Left=�6D   �� 56      -   5        $��tbContactNumber��56   *   $��1��6    .  ��6B   �� $ Number�57      59      � 5:   
   $��2��6   $	��5,5q�6   $
��Auto��6)    5;      �� 5<      $
��Auto �6#   �� 5=      $
��Auto �6#   �� 5>      $
��Auto �6#   �� 5?      $
��Auto �6#   .+�5A      �� -   5        $��LineSummaryCheckBox��5A      $��Center=�62    d� 	IsChecked 6M   �� $ LineSummary�	 �� Mode$	 TwoWay=��� 5B      $��1��6   $�� Hourly Summary ��6%   $
��Left=�6@   �� 5C      -   5        $��ShiftSummaryCheckBox��5C   '   $��2��6   $��Center=�6C    6^   �� $ ShiftSummary�$	 TwoWay=��� 5D      $��3��6   $�� Shift Summary ��6%   $
��Left=�6?   5E      A� 5F   
   
 A� Columns$
 4��6   $��3��6"   .+�5G      �� -   5        $��lineGroupBox��5G      $��15,0,0,0q�6+   ��5I      �� 5J      $��LINE ASSOCIATION��6    
�������$��12(�6N   .��5L      )  #Microsoft.Windows.Controls.DataGrid  -   5        $��dgLine��5L   &   $
��Left=�6a   
��� � �$��0q�5M   1      AutoGenerateColumns .     HeadersVisibility$	 All=�5O   "   $   HorizontalScrollBarVisibility$
 Auto=�6;   "   VerticalScrollBarVisibility$
 Auto=�6`   $��Center=�5P   "      CanUserAddRows .     CanUserSortColumns .     CanUserDeleteRows .     CanUserReorderColumns .  ��5L   4   �� $ LineAssociation�   Columns 5V      7  1Microsoft.Windows.Controls.DataGridCheckBoxColumn  5W      4  .Microsoft.Windows.Controls.DataGridBoundColumn   Binding 68   �� $ IsAssociated�$	 TwoWay=�3  -Microsoft.Windows.Controls.DataGridTextColumn  5Y      /  )Microsoft.Windows.Controls.DataGridColumn   Header$
 LINE��6R      
Foreground
 �   �   FontSize$ 15(�6t    64   �� $
 Name�6t   5\      5^      �� 5`      -   5        $��shiftGroupBox��5`      $��15,0,0,0q�60   ��5a      �� 5b      $��SHIFT ASSOCIATION��6    
�������$��12(�6O   .��5d        -	   5        $��dgShift��5d   "   $
��Left=�6_   
��� � �$��0q�5e   1    .  $ Column=�5g   "   $��Center=�5h   "    .   .   .   .  ��5d   1   �� $ ShiftAssociation� 5m        5n       68   �� $ IsAssociated�$	 TwoWay=�  5p      $ SHIFT��6R   
 �   �$ 15(�6t    64   �� $
 Name�6t   5s      5u      �� 5w      -
   5        $��departmentGroupBox��5w      $��15,0,0,0q�65   ��5x      �� 5y      $��DEPARTMENT ASSOCIATION��6    
�������$��12(�6T   .��5{        -   5        $��dgDepartment��5{   "   $
��Left=�6i   
��� � �$��0q�5|   1    .  $ Column=�5~   "   $��Center=�5   "    .   .   .   .  ��5{   6   �� $ DepartmentAssociation� 5�        5�       68   �� $ IsAssociated�$	 TwoWay=�  5�      $ 
DEPARTMENT��6R   
 �   �$ 15(�6y    64   �� $
 Name�6y   5�      5�      �� 5�      -   5        $��escalationGroupBox��5�      $��15,0,0,0q�65   ��5�      �� 5�      $��ESCALATION ASSOCIATION��6    
�������$��12(�6T   .��5�        -   5        $��dgEscalation��5�   "   $
��Left=�6i   
��� � �$��0q�5�   1    .  $ Column=�5�   "   $��Center=�5�   "    .   .   .   .  ��5�   6   �� $ EscalationAssociation� 5�        5�       6;   �� $ IsAssociated�$	 TwoWay=�  5�      $ 
ESCALATION��6S   
 �   �$ 15(�6z    65   �� $
 Name�6z   5�      5�      5�      5�      5�      