   M S B A M L   `   `   `  ���� J  FDetectionSystem, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null1clr-namespace:DetectionSystemDetectionSystem      DetectionSystem.MapWindow   R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35\ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35^ ZWindowsFormsIntegration, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35F 9http://schemas.microsoft.com/winfx/2006/xaml/presentation     R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     @d2http://schemas.microsoft.com/expression/blend/2008     Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     X TXceed.Wpf.Toolkit, Version=4.0.0.0, Culture=neutral, PublicKeyToken=3e4669d2f30244f44xctk)http://schemas.xceed.com/wpf/xaml/toolkit  )localclr-namespace:DetectionSystem    � Title$  Window1��$	��450��$	��800��.��� .+�� $
��Left=�$	��420��$	��Top=�$	��794��$��
0,0,-0.4,0q� � ColumnDefinitions �� $
��403* ��� $
��391* �.+��� -   $��
timeSlider��$��73,323,31.4,0q� �� TickPlacement$ BottomRight=�$��24��$	��Top=�$y�Hidden=� �� IsSnapToTickEnabled . �� -   $��mapOfDevices��$
��Left=�$��26,65,0,125q�$	��400�����	#FFF0F0F0$��2��$	��230���� $��1��$
��Left=�$	��230��$��28.6,65,0,0q�$	��Top=�$	��342��.@��� $��Most Recent Position��.��� ���	#FF0A003A �� $	��46* ��� $	��50* ��� $	��49* ��� $
��190* �.+��� -   $��
Update Map��$��0.663,133,94.737,0q�$��29��$	��Top=�$��2��$��2���� $
��Left=�$��	10,35,0,0q�$
��Wrap=�$	��Top=�$��4��$	��315��
������� �� TextAlignment$ Center=��� $A�;If you want to track the position of a single device, enter���� �� $	�its���� $*�$ mac address in the text field below���� -   $��macBox��$��3��$��1��$
��Left=�$��24��$��50.863,87,0,0q� �� TextWrapping$
 Wrap=�$	��Top=�$	��143�� �� TextAlignment$ Center=��� $��	Animation��.��� ���	#FF0A003A.+��� $��Begin��$
��Left=�$��	10,22,0,0q�$	��Top=�
��������� $	��End��$
��Left=�$��	10,53,0,0q�$	��Top=�
��������� $��Granularity (Seconds)��$
��Left=�$��	10,84,0,0q�$	��Top=�
�������&   Xceed.Wpf.Toolkit.DateTimePicker  -   $��beginDateTime��$
��Left=�$��
149,22,0,0q�$	��Top=�$	��172��$��25��&   Xceed.Wpf.Toolkit.DateTimeUpDown   Format$ Custom=�   FormatString$ yyyy-MM-dd HH:mm:ss��  -   $��lastDateTime��$
��Left=�$��
149,54,0,0q�$	��Top=�$	��172��$��25��$ Custom=�$ yyyy-MM-dd HH:mm:ss��%  Xceed.Wpf.Toolkit.IntegerUpDown  -   $��granularityBox��$
��Left=�$��
149,84,0,0q�$	��Top=�$��25��$	��154���� -   $��Generate Animation��$��99,144,98.2,0q�$	��Top=�$��28������ 	 �� ImageSource$	 
app_bg.png��