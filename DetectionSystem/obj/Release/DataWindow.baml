   M S B A M L   `   `   `  ���� U  QLiveCharts.Wpf, Version=0.9.7.0, Culture=neutral, PublicKeyToken=0bc1f845d1ebb8dfG4clr-namespace:LiveCharts.Wpf;assembly=LiveCharts.WpfLiveCharts.Wpf  J FDetectionSystem, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null1clr-namespace:DetectionSystemDetectionSystem     DetectionSystem.DataWindow   R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35\ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35^ ZWindowsFormsIntegration, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35F 9http://schemas.microsoft.com/winfx/2006/xaml/presentation     R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     @d2http://schemas.microsoft.com/expression/blend/2008     Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     X TXceed.Wpf.Toolkit, Version=4.0.0.0, Culture=neutral, PublicKeyToken=3e4669d2f30244f44xctk)http://schemas.xceed.com/wpf/xaml/toolkit  >lvc4clr-namespace:LiveCharts.Wpf;assembly=LiveCharts.Wpf  )localclr-namespace:DetectionSystem    � Title$  
DataWindow��$	��850��$
��1400�� � 
ResizeMode$ NoResize=� � WindowStartupLocation$ CenterScreen=�.��� .+��� $	��Top=�$��Courier New)�$
��Bold"�$��18(�
�������$��0,0,0,0q�.��Server Console�� -   $��stdout2��$	��200��$	��700��
���   �
�������$��Courier New)�$��12(�" �� VerticalScrollBarVisibility$
 Auto=�$ �� HorizontalScrollBarVisibility$
 Auto=���.  �� TextWrapping$
 Wrap=�$	��Top=�$��20,40,20,20q� �� MaxLines$ 8�� �� 
IsReadOnly . .���� $
��Left=�$	��700��$��139,300,0,61q�.@��� $��Devices's map��.��� 
���   �.+�#   LiveCharts.Wpf.CartesianChart  -   $��devicesChart��$	��400��$	��500��$
��Left=�$��0,0,0,0q�&    LiveCharts.Wpf.Charts.Base.Chart   Series �� *SeriesCollection+	   AxisX	    LiveCharts.Wpf.Axis  
   Title$

 Time��   Labels �� *	LabelsDev+   AxisY      MinValue$ 0Q�$
 Devices��   LabelFormatter �� *	Formatter+   	Separator    LiveCharts.Wpf.Separator  {�. � $	��200��$	��300��$��510,60,30,60q�.+��� $��0��$��0��$	��150��$	��Top=�$
��Left=�$��Courier New)�$
��Bold"�$��12(�
�������$��0,5,0,0q�.��Start Time:&   Xceed.Wpf.Toolkit.DateTimePicker  -   $��StartTimePicker��$��
0,30,0,235q�&   Xceed.Wpf.Toolkit.DateTimeUpDown   Format$ Custom=�   FormatString$ yyyy-MM-dd HH:mm:ss��$��35��$	��155��$
��Left=��� $��0��$��0��$	��150��$	��Top=�$
��Left=�$��Courier New)�$
��Bold"�$��12(�
�������$��0,70,0,0q�.��	End Time:  -   $��StopTimePicker��$��
0,95,0,171q�$ Custom=�$ yyyy-MM-dd HH:mm:ss��$��35��$	��155��$
��Left=��� $��0��$��0��$	��150��$	��Top=�$
��Left=�$��Courier New)�$
��Bold"�$��12(�
�������$��	0,140,0,0q�.��Granularity (s):%  Xceed.Wpf.Toolkit.IntegerUpDown  -   $��GranularityPicker��$��45,165,0,100q��  �Xceed.Wpf.Toolkit.Primitives.UpDownBase`1[[System.Nullable`1[[System.Int32, mscorlib, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089]], mscorlib, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089]]   Minimum$ 1X�   Value$ 1X�$��35��$��56�� J� RenderTransformOrigin$ -0.391,0.571&�$
��Left=��� -   $��update_chart��$��0��$��0��$	��100��$��20��
���   �$��Courier New)�$��12(�$	��Top=�$
��Left=�$��
20,225,0,0q�.��Update�� $��
Statistics��.��� 
���   �.+�  -   $��frequenciesChart��$	��400��$	��500��$
��Left=�$��0,0,0,0q� �� *ColumnCollection+	   $

 MACs�� �� *ColumnLabels+   $
 Quantity��$ 0Q� �� *ColumnFormatter+� $	��200��$��510,61,-15.6,30.6q�.+��� $��0��$��0��$	��150��$	��Top=�$
��Left=�$��Courier New)�$
��Bold"�$��12(�
�������$��0,5,0,0q�.��Start Time:  -   $��StartTimePickerCol��$��
0,30,0,265q�$ Custom=�$ yyyy-MM-dd HH:mm:ss��$��35��$	��155��$
��Left=��� $��0��$��0��$	��150��$	��Top=�$
��Left=�$��Courier New)�$
��Bold"�$��12(�
�������$��0,70,0,0q�.��	End Time:  -	   $��StopTimePickerCol��$��
0,95,0,200q�$ Custom=�$ yyyy-MM-dd HH:mm:ss��$��35��$	��155��$
��Left=��� $��0��$��0��$	��150��$	��Top=�$
��Left=�$��Courier New)�$
��Bold"�$��12(�
�������$��	0,137,0,0q�.��Number of devices:  -
   $��DevNumPickerCol��$��45,161,0,134q�$ 1X�$ 1X�$��35��$��56��$ -0.391,0.571&�$
��Left=��� $��0��$��0��$	��150��$	��Top=�$
��Left=�$��Courier New)�$
��Bold"�$��12(�
�������$��	0,201,0,0q�.��Granularity (s):  -   $��GranularityPickerStack��$��45,225,0,70q�$ 1X�$ 1X�$��35��$��56��$ -0.391,0.571&�$
��Left=��� -   $��update_chart_col��$��0��$��0��$	��100��$��20��
���   �$��Courier New)�$��12(�$	��Top=�$
��Left=�$��
19,280,0,0q�.��Update�� $��Hidden Devices��.��� 
���   �.+��� -   $��	mac_label��$	��400��$	��100��$
��Left=�$��50, 0, 0, 150q�$
��Bold"�$��16(�
�������.��!Local MAC addresses detected: 0�� -   $��device_label��$	��400��$	��100��$
��Left=�$��50, 50, 0, 150q�$
��Bold"�$��16(�
�������.��Real devices estimation: 0�� -   $��
risk_label��$	��400��$	��100��$
��Left=�$��50, 100, 0, 150q�$
��Bold"�$��16(�
�������.��Estimation risk: 0 %� $	��200��$��510,61,-15.6,30.6q�.+��� $��0��$��0��$	��150��$	��Top=�$
��Left=�$��Courier New)�$
��Bold"�$��12(�
�������$��0,5,0,0q�.��Start Time:  -   $��StartTimePickereErr��$��
0,30,0,265q�$ Custom=�$ yyyy-MM-dd HH:mm:ss��$��35��$	��155��$
��Left=��� $��0��$��0��$	��150��$	��Top=�$
��Left=�$��Courier New)�$
��Bold"�$��12(�
�������$��0,70,0,0q�.��	End Time:  -   $��StopTimePickerErr��$��
0,95,0,200q�$ Custom=�$ yyyy-MM-dd HH:mm:ss��$��35��$	��155��$
��Left=��� -   $��update_chart_err��$��0��$��0��$	��100��$��20��
���   �$��Courier New)�$��12(�$	��Top=�$
��Left=�$��
19,160,0,0q�.��Update� $��Right=�$	��563��$��Bottom=�$	��794��$��0,0,-23.4,0q� � ColumnDefinitions �� $
��105* ��� $
��297* ��� $
��391* �.+��� -   $��
timeSlider��$��255.6,273,99.6,0q� �� TickPlacement$ BottomRight=�$	��230��$y�Hidden=�$	��Top=� �� IsSnapToTickEnabled . $��2�� �� Orientation$ Vertical=��� -   $��mapOfDevices��$
��Left=�$��139.2,273,0,60q�$	��400��$��2��$	��230��$��1������  �� ImageSource$ 
floorb.png���� $��1��$
��Left=�$	��230��$��139.2,43,0,0q�$	��Top=�$	��400��$��2��.@��� $��Most Recent Position��.��� 
���   � �� $	��46* ��� $	��50* ��� $	��49* ��� $
��190* �.+��� -   $��
Update Map��$��0.2,133,95.4,0q�$��29��$	��Top=�$��2��$��2���� $
��Left=�$��	39,34,0,0q�$
��Wrap=�$	��Top=�$��4��$	��315��
������� �� TextAlignment$ Center=��� $A�;If you want to track the position of a single device, enter���� �� $	�its���� $*�$ mac address in the text field below���� -   $��macBox��$��2��$��2��$
��Left=�$��24��$��22.2,86,0,0q�$
 Wrap=�$	��Top=�$	��143�� �� TextAlignment$ Center=��� $��	Animation��.��� 
���   � �� $	��63* ��� $
��272* �.+��� $��Begin��$
��Left=�$��	33,22,0,0q�$	��Top=�
��������� $	��End��$
��Left=�$��	33,53,0,0q�$	��Top=�
��������� $��Granularity (Seconds)��$
��Left=�$��	33,84,0,0q�$	��Top=�
�������$��2��  -   $��beginDateTime��$
��Left=�$��108.6,22,0,0q�$	��Top=�$	��172��$��25��$ Custom=�$ yyyy-MM-dd HH:mm:ss��$��1��  -   $��lastDateTime��$
��Left=�$��108.6,54,0,0q�$	��Top=�$	��172��$��25��$ Custom=�$ yyyy-MM-dd HH:mm:ss��$��1��  -   $��granularityBox��$ 1X�$
��Left=�$��108.6,84,0,0q�$	��Top=�$��25��$	��154��$��1���� -   $��Generate Animation��$��36,144,98.2,0q�$	��Top=�$��28��$��1������ $ 
app_bg.png��