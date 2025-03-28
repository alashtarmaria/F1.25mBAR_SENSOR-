# F1.25mBAR_SENSOR

🔍 Sensör Özeti:
📌 Model: Sensirion, ±1.25 mbar diferansiyel
📌 Çıkış tipi: Analog çıkış (Vout = 190 × DP)
📌 Opamp ile kuvvetlendirilmiş
📌 Çıkış ADC CH4'e bağlı

Vout ≈ 190 × DP  → (Vout Volt, DP Pascal)
DP (Pa) = Vout / 190


1 mbar = 100 Pa
⇒ DP (mbar) = (Vout / 190) / 100 = Vout / 19000

🎯 Formül:
Pressure_mbar = Voltage / 0.019
(yani Vout / (190 × 0.1))
