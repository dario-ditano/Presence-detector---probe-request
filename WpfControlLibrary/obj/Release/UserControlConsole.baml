   M S B A M L   `   `   `  ���� L  HWpfControlLibrary, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null5clr-namespace:WpfControlLibraryWpfControlLibrary  U QLiveCharts.Wpf, Version=0.9.7.0, Culture=neutral, PublicKeyToken=0bc1f845d1ebb8dfG4clr-namespace:LiveCharts.Wpf;assembly=LiveCharts.WpfLiveCharts.Wpf *    $WpfControlLibrary.UserControlConsole   R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35\ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35D 9http://schemas.microsoft.com/winfx/2006/xaml/presentation    R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     @d2http://schemas.microsoft.com/expression/blend/2008     1localclr-namespace:WpfControlLibrary     Dlvc4clr-namespace:LiveCharts.Wpf;assembly=LiveCharts.Wpf     X TXceed.Wpf.Toolkit, Version=4.0.0.0, Culture=neutral, PublicKeyToken=3e4669d2f30244f44xctk)http://schemas.xceed.com/wpf/xaml/toolkit  -     � 	Resources     RoundButtonTemplate �� 
TargetType �� Property �� Value �� 
TargetType �� CornerRadius  RoundButtonTemplateViolet  RoundButtonTemplateGrey��%�  &	        &	 �     &	 >    �� " ��.���  � �� �	#FF33335A��  � ��
 �������  � �� �� " ��.R��� $ 5��#��z��$��1q�._��� $��Center=�$��Center=��� " ��.���  � �� �	#FFFF466E��  � ��
 �������  � �� �� " ��.R��� $ 8��#��z��$��1q�._��� $��Center=�$��Center=��� " ��.���  � ��
 �������  � ��
 �������  � �� �� " ��.R��� $ 5��#��z��$��1q�._��� $��Center=�$��Center=�.��� -    �Name$ 	btn_info1��$��0q�$
��1400��$	��800�� � ColumnDefinitions �� $	��900 ��� $	��10* � � RowDefinitions �� $
��800* ��� $��1300* �.+��� -   $��stdout2��$	��230��$	��700��
���   �
�������$��Courier New)�$��12(�"	 �� VerticalScrollBarVisibility$
	 Auto=�$
 �� HorizontalScrollBarVisibility$

 Auto=���.  �� TextWrapping$
 Wrap=�$	��Top=�$��49,30,151,0q� �� MaxLines$ 8�� �� 
IsReadOnly . .���� $
��Left=�$	��700��$	��480��$��50,270,0,50q�$��2������ .@��� $��Devices Stats��$
��Bold"�.��� ���	#E5000000$	��700��$	��450�� �� $
��121* ��� $
��579* �.+�#  LiveCharts.Wpf.CartesianChart  -   $��devicesChart��$	��666��$
��Left=�$��	0,90,0,25q�$��2��&   LiveCharts.Wpf.Charts.Base.Chart   Series �� *SeriesCollection+   AxisX   LiveCharts.Wpf.Axis     Title$
 Time��   Labels �� *	LabelsDev+   AxisY      MinValue$ 0Q�$ Devices��   LabelFormatter �� *	Formatter+   	Separator   LiveCharts.Wpf.Separator  {�. � $��10,-10,180,117q�$��2�� �� $��5* ��� $	��52* ��� $
��198* �.+��� $��0��$��0��$	��150��$	��Top=�$
��Left=�$
��Bold"�$��12(�
�������$��8,23,0,0q� J� RenderTransformOrigin$ 	0.307,0.5&�$��3��.��Start Time:&   Xceed.Wpf.Toolkit.DateTimePicker  -   $��StartTimePicker��$��2,47,0,260.6q�&   Xceed.Wpf.Toolkit.DateTimeUpDown   Format$ Custom=�   FormatString$ yyyy-MM-dd HH:mm:ss��$��35��$	��145��$
��Left=�$ 1.233,-0.114&�$��2��$��1��$��Normal"����	#FF383847�� $��2��$��0��$	��150��$	��Top=�$
��Left=�$
��Bold"�$��12(�
�������$��47.4,23,0,0q�$ 
1.277,-2.4&�.��	End Time:  -   $��StopTimePicker��$��51.4,47,0,260.6q�$ Custom=�$ yyyy-MM-dd HH:mm:ss��$��35��$	��145��$
��Left=�$��2��$��Normal"��� $��2��$��0��$	��150��$	��Top=�$
��Left=�$
��Bold"�$��12(�
�������$��200.4,23,0,0q�$��Step(s):��%  Xceed.Wpf.Toolkit.IntegerUpDown  -   $��GranularityPicker��$��204.4,47,0,260.6q��  �Xceed.Wpf.Toolkit.Primitives.UpDownBase`1[[System.Nullable`1[[System.Int32, mscorlib, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089]], mscorlib, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089]]   Minimum$ 1X�   Value$ 1X�$��35��$��56��$ -0.391,0.571&�$
��Left=�$��2��$��Normal"��� -   $��update_chart��#��[ $��2��$��0��$��30��
�������$��Segoe UI)�$��12(�$	��Top=�$��414.4,47,-139.4,0q�$ 
0.057,0.64&�$
��Bold"�.��Update�� -	    ��Name$	 ch1��$��Complete Mode��$��1��$
��Left=�$��279.2,48,0,0q�$	��Top=�
�������$
��Bold"�$��13(��� $��1��$
��Left=�$��539.2,415,0,0q�$	��Top=�$��30��$��30��$ 1.257,0.563&�����  �� ImageSource$" baseline_help_white_18dp.png���� $��Packets Stats��$
��Bold"�.��� ���	#E5000000 �� $	��32* ��� $
��169* ��� $
��494* �.+�  -
   $��frequenciesChart��$	��666��$
��Left=�$��	0,90,0,30q�$��3�� �� *ColumnCollection+   $
 MACs�� �� *ColumnLabels+   $ Quantity��$ 0Q� �� *ColumnFormatter+� $��23,10,10.4,291.6q�$��3��.+��� $��0��$��0��$	��150��$	��Top=�$
��Left=�$��Courier New)�$
��Bold"�$��12(�
�������$��0,4,0,0q�.��Start Time:�� $��0��$��0��$	��150��$	��Top=�$
��Left=�$��Courier New)�$
��Bold"�$��12(�
�������$��	170,4,0,0q�.��	End Time:  -   $��StopTimePickerCol��$��170,30,0,85q�$ Custom=�$ yyyy-MM-dd HH:mm:ss��$��35��$	��155��$
��Left=��� $��0��$��0��$	��150��$	��Top=�$
��Left=�$��Courier New)�$
��Bold"�$��12(�
�������$��	434,4,0,0q�$��	Step (s):��  -   $��GranularityPickerStack��$��434,30,0,85q�$ 1X�$ 1X�$��35��$��56��$ -0.391,0.571&�$
��Left=��� -   $��update_chart_col��#��[ $��0��$��0��$	��120��$��30��
�������$��Courier New)�$��12(�$	��Top=�$
��Left=�$��
531,33,0,0q�.��Update  -     Name$ StartTimePickerCol��$��	0,30,0,85q�$ Custom=�$ yyyy-MM-dd HH:mm:ss��$��35��$	��155��$
��Left=��� $��Hidden Devices��$
��Bold"�.��� ���	#E5000000.+��� -   $��	mac_label��$	��400��$	��100��$
��Left=�$��50, 0, 0, 150q�$
��Bold"�$��16(�
�������.��!Local MAC addresses detected: 0�� -   $��device_label��$	��400��$	��100��$
��Left=�$��50, 50, 0, 150q�$
��Bold"�$��16(�
�������.��Real devices estimation: 0�� -   $��
risk_label��$	��400��$	��100��$
��Left=�$��50, 100, 0, 150q�$
��Bold"�$��16(�
�������.��Estimation risk: 0 %� $	��200��$��510,61,-15.6,30.6q�.+��� $��0��$��0��$	��150��$	��Top=�$
��Left=�$��Courier New)�$
��Bold"�$��12(�
�������$��0,5,0,0q�.��Start Time:  -   $��StartTimePickereErr��$��
0,30,0,265q�$ Custom=�$ yyyy-MM-dd HH:mm:ss��$��35��$	��155��$
��Left=��� $��0��$��0��$	��150��$	��Top=�$
��Left=�$��Courier New)�$
��Bold"�$��12(�
�������$��0,70,0,0q�.��	End Time:  -   $��StopTimePickerErr��$��
0,95,0,200q�$ Custom=�$ yyyy-MM-dd HH:mm:ss��$��35��$	��155��$
��Left=��� -   $��update_chart_err��$��0��$��0��$	��100��$��30��#��[ 
�������$��Courier New)�$��12(�$	��Top=�$
��Left=�$��
19,160,0,0q�.��Update�� ���	#FFF4F4F5$
��Left=�$	��388��$��811,289,0,0q�
���   �$	��Top=�$	��458��$��2��$��2��� $��Right=�$	��720��$��Bottom=�$	��794��$��0,-28,0,108q�$��2��$��2�� �� $
��105* ��� $
��297* ��� $
��391* �.+��� -   $��
timeSlider��$y�Hidden=�$��128.2,706,160.6,-22q� �� TickPlacement$ BottomRight=�$��36��$	��Top=� �� IsSnapToTickEnabled . $��1��  �� Orientation$  
Horizontal=����	#7F000000$��2���� -   $��mapOfDevices��$
��Left=�$��104.2,313,0,10q�$	��450��$��2��$	��380��$��1������ $ 
floorb.png���� $��1��$
��Left=�$	��230��$��
128,90,0,0q�$	��Top=�$	��400��$��2��.@��� $��Most Recent Position��.��� ���	#E5000000 �� $	��54* ��� $	��59* ��� $	��37* ��� $	��20* ��� $
��224* �.+��� -   #��[  $��
Update Map��$��0.734,133,94.866,0q�$��30��$	��Top=�$��2��$��3���� $
��Left=�$��	39,34,0,0q�$
��Wrap=�$	��Top=�$��5��$	��315��
�������! �� TextAlignment$! Center=��� $A�;If you want to track the position of a single device, enter���� �� $	�its���� $*�$ mac address in the text field below���� -   $��macBox��$��3��$��2��$
��Left=�$��24��$��21.734,86,0,0q�$
 Wrap=�$	��Top=�$	��143��" �� TextAlignment$" Center=��� $��	Animation��.��� ���	#E5000000 �� $	��63* ��� $
��272* �.+��� $��Begin��$
��Left=�$��	33,22,0,0q�$	��Top=�
��������� $	��End��$
��Left=�$��	33,53,0,0q�$	��Top=�
��������� $��Granularity (Seconds)��$
��Left=�$��	33,84,0,0q�$	��Top=�
�������$��2��  -   $��beginDateTime��$
��Left=�$��108.6,22,0,0q�$	��Top=�$	��172��$��25��$ Custom=�$ yyyy-MM-dd HH:mm:ss��$��1��  -   $��lastDateTime��$
��Left=�$��108.6,54,0,0q�$	��Top=�$	��172��$��25��$ Custom=�$ yyyy-MM-dd HH:mm:ss��$��1��  -   $��granularityBox��$ 1X�$
��Left=�$��108.6,84,0,0q�$	��Top=�$��25��$	��154��$��1���� -   #��[  $��Generate Animation��$��36,144,98.2,0q�$	��Top=�$��30��$��1������ $ 
app_bg.png��