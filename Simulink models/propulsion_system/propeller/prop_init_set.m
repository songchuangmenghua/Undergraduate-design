%% Initialize propeller parameters
%{
prop_performance_data.txt:
line 1: air speed 1kRPM
line 2: air speed 2kRPM
line 3: air speed 3kRPM
line 4: air speed 4kRPM
line 5: air speed 5kRPM
line 6: advance ratio 1kRPM
line 7: advance ratio 2kRPM
line 8: advance ratio 3kRPM
line 9: advance ratio 4kRPM
line 10: advance ratio 5kRPM
line 11: thrust coefficient 1kRPM
line 12: thrust coefficient 2kRPM
line 13: thrust coefficient 3kRPM
line 14: thrust coefficient 4kRPM
line 15: thrust coefficient 5kRPM
line 16: power coefficient 1kRPM
line 17: power coefficient 2kRPM
line 18: power coefficient 3kRPM
line 19: power coefficient 4kRPM
line 20: power coefficient 5kRPM
line 21: propeller efficiency 1kRPM
line 22: propeller efficiency 2kRPM
line 23: propeller efficiency 3kRPM
line 24: propeller efficiency 4kRPM
line 25: propeller efficiency 5kRPM
line 26: thrust 1kRPM
line 27: thrust 2kRPM
line 28: thrust 3kRPM
line 29: thrust 4kRPM
line 30: thrust 5kRPM
line 31: torque 1kRPM
line 32: torque 2kRPM
line 33: torque 3kRPM
line 34: torque 4kRPM
line 35: torque 5kRPM
%}
%% Initialize
interp_air_speed = [0 : 0.5 : 50];
RPM = [1000 2000 3000 4000 5000]';
prop_performance_data = load('prop_performance_data.txt');

%% Advance ratio
adv_ratio_1000rpm = interp1(prop_performance_data(1 , :) , prop_performance_data(6 , :), interp_air_speed , 'next');
adv_ratio_2000rpm = interp1(prop_performance_data(2 , :) , prop_performance_data(7 , :), interp_air_speed , 'next');
adv_ratio_3000rpm = interp1(prop_performance_data(3 , :) , prop_performance_data(8 , :), interp_air_speed , 'next');
adv_ratio_4000rpm = interp1(prop_performance_data(4 , :) , prop_performance_data(9 , :), interp_air_speed , 'next');
adv_ratio_5000rpm = interp1(prop_performance_data(5 , :) , prop_performance_data(10 , :), interp_air_speed , 'next');

adv_ratio_total = [adv_ratio_1000rpm; adv_ratio_2000rpm; adv_ratio_3000rpm; adv_ratio_4000rpm; adv_ratio_5000rpm];
adv_ratio_total(isnan(adv_ratio_total)) = 0;

%% Thrust coefficient
Ct_1000rpm = interp1(prop_performance_data(1 , :) , prop_performance_data(11 , :), interp_air_speed , 'next');
Ct_2000rpm = interp1(prop_performance_data(2 , :) , prop_performance_data(12 , :), interp_air_speed , 'next');
Ct_3000rpm = interp1(prop_performance_data(3 , :) , prop_performance_data(13 , :), interp_air_speed , 'next');
Ct_4000rpm = interp1(prop_performance_data(4 , :) , prop_performance_data(14 , :), interp_air_speed , 'next');
Ct_5000rpm = interp1(prop_performance_data(5 , :) , prop_performance_data(15 , :), interp_air_speed , 'next');

Ct_total = [Ct_1000rpm; Ct_2000rpm; Ct_3000rpm; Ct_4000rpm; Ct_5000rpm];
Ct_total(isnan(Ct_total)) = 0;

%% Power coefficient
Cp_1000rpm = interp1(prop_performance_data(1 , :) , prop_performance_data(16 , :), interp_air_speed , 'next');
Cp_2000rpm = interp1(prop_performance_data(2 , :) , prop_performance_data(17 , :), interp_air_speed , 'next');
Cp_3000rpm = interp1(prop_performance_data(3 , :) , prop_performance_data(18 , :), interp_air_speed , 'next');
Cp_4000rpm = interp1(prop_performance_data(4 , :) , prop_performance_data(19 , :), interp_air_speed , 'next');
Cp_5000rpm = interp1(prop_performance_data(5 , :) , prop_performance_data(20 , :), interp_air_speed , 'next');

Cp_total = [Cp_1000rpm; Cp_2000rpm; Cp_3000rpm; Cp_4000rpm; Cp_5000rpm];
Cp_total(isnan(Cp_total)) = 0;

%% Proprller efficiency
prop_eff_1000rpm = interp1(prop_performance_data(1 , :) , prop_performance_data(21 , :), interp_air_speed , 'next');
prop_eff_2000rpm = interp1(prop_performance_data(2 , :) , prop_performance_data(22 , :), interp_air_speed , 'next');
prop_eff_3000rpm = interp1(prop_performance_data(3 , :) , prop_performance_data(23 , :), interp_air_speed , 'next');
prop_eff_4000rpm = interp1(prop_performance_data(4 , :) , prop_performance_data(24 , :), interp_air_speed , 'next');
prop_eff_5000rpm = interp1(prop_performance_data(5 , :) , prop_performance_data(25 , :), interp_air_speed , 'next');

prop_eff_total = [prop_eff_1000rpm; prop_eff_2000rpm; prop_eff_3000rpm; prop_eff_4000rpm; prop_eff_5000rpm];
prop_eff_total(isnan(prop_eff_total)) = 0;

%% Thrust
thrust_1000rpm = interp1(prop_performance_data(1 , :) , prop_performance_data(21 , :), interp_air_speed , 'next');
thrust_2000rpm = interp1(prop_performance_data(2 , :) , prop_performance_data(22 , :), interp_air_speed , 'next');
thrust_3000rpm = interp1(prop_performance_data(3 , :) , prop_performance_data(23 , :), interp_air_speed , 'next');
thrust_4000rpm = interp1(prop_performance_data(4 , :) , prop_performance_data(24 , :), interp_air_speed , 'next');
thrust_5000rpm = interp1(prop_performance_data(5 , :) , prop_performance_data(25 , :), interp_air_speed , 'next');

thrust_total = [thrust_1000rpm; thrust_2000rpm; thrust_3000rpm; thrust_4000rpm; thrust_5000rpm];
thrust_total(isnan(thrust_total)) = 0;

%% Torque
torque_1000rpm = interp1(prop_performance_data(1 , :) , prop_performance_data(21 , :), interp_air_speed , 'next');
torque_2000rpm = interp1(prop_performance_data(2 , :) , prop_performance_data(22 , :), interp_air_speed , 'next');
torque_3000rpm = interp1(prop_performance_data(3 , :) , prop_performance_data(23 , :), interp_air_speed , 'next');
torque_4000rpm = interp1(prop_performance_data(4 , :) , prop_performance_data(24 , :), interp_air_speed , 'next');
torque_5000rpm = interp1(prop_performance_data(5 , :) , prop_performance_data(25 , :), interp_air_speed , 'next');

torque_total = [torque_1000rpm; torque_2000rpm; torque_3000rpm; torque_4000rpm; torque_5000rpm];
torque_total(isnan(torque_total)) = 0;
