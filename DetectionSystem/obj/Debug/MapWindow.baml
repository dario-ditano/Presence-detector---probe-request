   M S B A M L   `   `   `  ����J  FDetectionSystem, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null1clr-namespace:DetectionSystemDetectionSystem  5   	       DetectionSystem.MapWindow   5   	   \ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35D 9http://schemas.microsoft.com/winfx/2006/xaml/presentation    5   	   R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5   	   @d2http://schemas.microsoft.com/expression/blend/2008     5   	   Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     5   	   X TXceed.Wpf.Toolkit, Version=3.4.0.0, Culture=neutral, PublicKeyToken=3e4669d2f30244f44xctk)http://schemas.xceed.com/wpf/xaml/toolkit  5   	   )localclr-namespace:DetectionSystem  5   	     � Title$  Window1��5	   	   $	��450��6   $	��800��6&   .��5
      � .+�5   
   � $
��Left=�6   $	��420��6*   $	��Top=�67   $	��794��6O   $��
0,0,-0.4,0q�6[    � ColumnDefinitions 5      �� 5      $
��403* �6#   �� 5      $
��391* �6#   .+�5      �� -   5        $��
timeSlider��5      $��73,323,31.4,0q�6'    �� TickPlacement$ BottomRight=�6>   $��24��6Z   $	��Top=�6f   $y�Hidden=�6�    �� IsSnapToTickEnabled . 6�   �� 5      -   5        $��mapOfDevices��5      $
��Left=�6)   $��26,65,0,125q�6D   $	��400��6Y   ���	#FFF0F0F0$��2��6|   $	��230��6�   �� 5      $��1��6   $
��Left=�6)   $	��230��6D   $��28.6,65,0,0q�6Q   $	��Top=�6f   $	��342��6~   .@�5      �� $��Most Recent Position��6   .��5      � ���	#FF0A003A 5      �� 5      $	��46* �6/   �� 5      $	��50* �6/   �� 5      $	��49* �6/   �� 5      $
��190* �6/   .+�5      �� -   5        $��
Update Map��5   !   $��0.663,133,94.737,0q�66   $��29��6g   $	��Top=�6s   $��2��6�   $��2��6�   �� 5      $
��Left=�6$   $��	10,35,0,0q�6?   $
��Wrap=�6R   $	��Top=�6f   $��4��6~   $	��315��6�   
������� �� TextAlignment$ Center=�6�   �� 6�   $A�;If you want to track the position of a single device, enter��6�   �� 6  �� 6  $	�its��6"  �� 6/  $*�$ mac address in the text field below��63  6b  �� 5      -   5        $��macBox��5   "   $��3��60   $��1��6D   $
��Left=�6T   $��24��6o   $��50.863,87,0,0q�6{    �� TextWrapping$
 Wrap=�6�   $	��Top=�6�   $	��143��6�    �� TextAlignment$ Center=�6�   5      5      �� 5       $��	Animation��6   .��5!      � ���	#FF0A003A.+�5"      �� $��Begin��6    $
��Left=�60   $��	10,22,0,0q�6K   $	��Top=�6^   
�������6v   �� 5#      $	��End��6    $
��Left=�6.   $��	10,53,0,0q�6I   $	��Top=�6\   
�������6t   �� 5$      $��Granularity (Seconds)��6    $
��Left=�6@   $��	10,84,0,0q�6[   $	��Top=�6n   
�������6�   &   Xceed.Wpf.Toolkit.DateTimePicker  5%      -   5        $��beginDateTime��5%   .   $
��Left=�6C   $��
149,22,0,0q�6^   $	��Top=�6r   $	��172��6�   $��25��6�   &   Xceed.Wpf.Toolkit.DateTimeUpDown   Format$ Custom=�6�      FormatString$ yyyy-MM-dd HH:mm:ss��6�     5&      -   5        $��lastDateTime��5&   .   $
��Left=�6B   $��
149,54,0,0q�6]   $	��Top=�6q   $	��172��6�   $��25��6�   $ Custom=�6�   $ yyyy-MM-dd HH:mm:ss��6�   %  Xceed.Wpf.Toolkit.IntegerUpDown  5'      -   5        $��granularityBox��5'   -   $
��Left=�6C   $��
149,84,0,0q�6^   $	��Top=�6r   $��25��6�   $	��154��6�   �� 5(      -   5        $��Generate Animation��5(   !   $��99,144,98.2,0q�6>   $	��Top=�6m   $��28��6�   5)      5*      5+      5,      ��5.   
   �� 5/      	 �� ImageSource$	 
app_bg.png��6   51      52      