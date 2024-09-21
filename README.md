<p><H3>ESP32S3�ɂ��C���^�[�l�b�g���W�I</H3></p>
<p>
MP3�X�g���[����I2S�f�[�^�փf�R�[�h�ł���<a href="https://github.com/schreibfaul1/ESP32-audioI2S">���C�u����</a>�𗘗p�����ȒP�\���̃C���^�[�l�b�g���W�I�ł��B<br>
ESP32S3��I2S DAC�A�\�����u������ΐ���ł��܂��B�R�[�h��<a href="https://wiki.seeedstudio.com/xiao_esp32s3_getting_started/">Seeed Studio XIAO ESP32S3</a>�ɑΉ����Ă��܂��B<br>
Web�T�[�o�[�𓋍ڂ��Ă���̂ŁA�u���E�U���瑀��ł��܂��i�^�N�g�X�C�b�`�̓I�v�V�����j�B<br>
Arduino IDE�i2.1.0�j��<a href="https://github.com/espressif/arduino-esp32">ESP32�J�����i2.0.17�j</a>���K�v�ł��B
</p>

<p><strong>����</strong><br>
 �E�\�����ȒP�ň����B<br>
 �EWiFi�ڑ��B<br>
 �EMP3�X�g���[���̃C���^�[�l�b�g���W�I�ǂ���M�ł���B<br>
 �E�u���E�U���瑀��ł���i�������m�F���Ȃ���������Ƒ��삷��j�B�^�N�g�X�C�b�`�̐ڑ����\�i�R�[�h��̒�`�͂��Ă���j�B<br>
 �E�u���E�U�ɃA�[�e�B�X�g���A�Ȗ����\���i���{��j�����i�\�����u�ł͓��{��͕s�j�B<br>
 �E�C���^�[�l�b�g���W�I�ǂ̐ݒ�̓v���Z�b�g�܂���URL����͐ݒ�i�ݒ�͕ۑ������j�B<br>
 �E�����\���i1,2�s�ڂɐ؂�ւ��\���j�ƃX���[�v�^�C�}�[�i�U�O���j�t���B�X���[�v�^�C�}�[�̎c�莞�Ԃ͕\�����u��1�s�ڂɕ\���iSLPmm�j�����B<br>
</p>
<p><strong>H/W�\��</strong><br>
 �ESeeed Studio XIAO ESP32S3�i����ESP32S3�ł�GPIO�̐ڑ���ύX����Γ��삷��\������j<br>
 �E<a href="xiao_esp32s3_pcm5102_uda1334.JPG">UDA1334 I2S DAC �u���[�N�A�E�g��� �܂���PCM5102 I2S DAC �u���[�N�A�E�g���</a><br>
   ���ӓ_�Ƃ��āAPCM5102��̗��ʂ�<a href="xiao_esp32s3_jumper.JPG">1����4�̃n���__�W�����p�[</a>������A�ݒ�(3��H�A����L)���Ă����K�v�����邪�A�n���_�u���b�W�������������B<br>
   �Ȃ�PCM5102�̓X�s�[�J�[��炷�ɂ̓p���[�s���BUDA1334�͏����ʂŗǂ��Ȃ�OK�B<br>
 �ESD1306 128x64 OLED�\�����u<br>
</p>
<p>
<img src="xiao_esp32s3_iradio.jpg" width="300" height="380">
<p>�����XIAO ESP32S3��I2C�Ή��̕��𗬗p�B���퓮�쎞�͉�LED����S�b�Ԋu�œ_�ŁA�ʐ^�ł̓^�N�g�X�C�b�`�͖��ڑ�</p>
</p>
<p><strong>����</strong><br>
 �E�v���Z�b�g�����C���^�[�l�b�g���W�I�ǂ̐؂�ւ��i�u���E�U�^�^�N�g�X�C�b�`�j�B<br>
 �E���ʒ����i�u���E�U�^�^�N�g�X�C�b�`�j�B<br>
 �E�X���[�v�^�C�}�[��ON/OFF�i�u���E�U�^�^�N�g�X�C�b�`�j�B<br>
 �E�C���^�[�l�b�g���W�I�ǂ�URL���́A�ݒ�i�e�X�g�j�ƕۑ��i�u���E�U�j�B<br>
</p>
<p><strong>�ڑ�</strong><br>
<p>�e�R���|�[�l���g�̐ڑ��͈ȉ��̒ʂ�B<br>
</p>
<p>
I2S DAC
<table> 
<tr>
<td>I2S DAC(UDA1334)&nbsp;</td><td>I2S DAC(PCM5102)&nbsp;</td><td>ESP32S3</td>
</tr>
<tr>
<td>VIN</td><td>VIN</td><td>3V3</td>
</tr>
<tr>
<td>GND</td><td>GND</td><td>GND</td>
</tr>
<tr>
<td>WSEL</td><td>LCK</td><td>GPIO9</td>
</td>
<tr>
<td>DIN</td><td>DIN</td><td>GPIO3</td>
</tr>
<tr>
<td>BCLK</td><td>BCK</td><td>GPIO7</td>
</tr>
<tr>
<td>-</td><td>SCK</td><td>GND</td>
</tr>
</table>
</p>
<p>
I2C �\�����u
<table> 
<tr>
<td>I2C&nbsp;</td><td>ESP32S3</td>
</tr>
<tr>
<td>VCC</td><td>3V3</td>
</tr>
<tr>
<td>GND</td><td>GND</td>
</tr>
<tr>
<td>SCK</td><td>GPIO6</td>
<tr>
<tr>
<td>SDA</td><td>GPIO5</td>
<tr>
</table>
</P>
<p>
�^�N�g�X�C�b�`�i�{�^���j
���u���E�U���瑀��ł���̂Ŗ����Ă�OK�B
<table> 
<tr>
<td>�@�\&nbsp;</td><td>ESP32S3&nbsp;</td><td>�@�\</td>
</tr>
<tr>
<td>SLEEP</td><td>GPIO1</td></td><td>SLEEP�@�\��ON/OFF</td>
</tr>
<tr>
<td>VOL-</td><td>GPIO4</td><td>���ʏ��i�ŏ�0�A�X�e�b�v��5�j</td>
</tr>
<tr>
<td>VOL+</td><td>GPIO2</td><td>���ʑ�i�ő�50�j</td>
<tr>
<tr>
<td>STATION</td><td>GPIO8</td><td>�C���^�[�l�b�g���W�I�ǂ��u���ցv�؂�ւ��B�Ō�̏ꍇ�͐擪�ɖ߂�B</td>
<tr>
</table>
</p>
</p>
<p><strong>�C���X�g�[��</strong><br>
<ol>
<li>ArduinoIDE�ɂ����ă��C�u�����iESP32_audioI2S�Ȃǁj�𓱓�</li>
<li>�{�R�[�h���AZIP�`���Ń_�E�����[�h�i���쎞�̃I���W�i����ESP32_audioI2S�̃��C�u��������荞��ł���j</li>
<li>ArduinoIDE����xiao_esp32s3_ssd1306_inet_radio_master.ino���J��</li>
<li>ArduinoIDE�̃{�[�h�}�l�[�W������xiao_esp32s3��I��</li>
<li>�`����ssid = "xxxxxxxx"; �� password = "pppppppp"; ��2�s��<br>���p���Ă���WiFi�|�C���g�̂��̂ɏ���������</li>
<li>�u���؁E�R���p�C���v�ɐ���������A��U�A�u���O��t���ĕۑ��v���s��<br>
�i���C�u�������s���̃G���[���o����A�ǉ�����B�Ⴆ�΁ASD1306 OLED�\�����u�j
</li>
<li>��ɏq�ׂ�H/W�\���iXIAO ESP32S3�ƕ\�����u��DAC���K�v�j��p�ӂ��A�z����ڑ�����</li>
<li>PC��USB�P�[�u���Őڑ�����BXIAO ESP32S3��BOOT�{�^�����������܂܁ARESET�{�^���������ė����A����BOOT�{�^�������B����ŃA�b�v���[�h�҂��ɂȂ�B<br>�K����COM�ԍ��ŔF�������̂őI������iUSB�̃h���C�o���K�v�ȏꍇ����j</li>
<li>�u�A�b�v���[�h�v���s��</li>
<li>�������݂�����������ARESET�{�^���������΁A������J�n���A�󋵂��V���A�����j�^�i���邢�͕\�����u�j�ɕ\�������</li>
<li>WiFi�ڑ����AIP�A�h���X���V���A�����j�^�i���邢�͋N�����ɒZ���ԁA�\�����u�j�ɕ\�������B�u���E�U����"http://IP�A�h���X"��<br>�A�N�Z�X����ƈȉ��̉�ʂ��\�������<br>
</li>
</ol>
</p>
<p>
<p>�ڑ����̃C���^�[�l�b�g���W�I�ǂ�URL�A�A�[�e�B�X�g���A�Ȗ��AVOLUME�̒l�A�v���Z�b�g�ς̃C���^�[�l�b�g���W�I�ǂ�URL���\������Ă���<br>
</p>
<p>
<img src="xiao_esp32s3_iradio_web.png" width="600" height="400">
<p>�uControl Functions�v�ɂ͉��ʁA�X�e�[�V�����ύX�A�X���[�v�@�\�̊e����̃{�^��������B�uStation URL�v�Ɉꎞ�I�ȃC���^�[�l�b�g���W�I�ǂ�URL���w��ł��A<br>
�uTEST SET�v�{�^���������Ď�MOK�̏ꍇ�A�擪�̓��̓t�B�[���h�ɃG���g���ԍ��A2�Ԗڂ̓��̓t�B�[���h�ɃX�e�[�V�������i10�����ȓ��j���w�肵�A<br>
�uSAVE�v���N���b�N����ƁA�擪�̓��̓t�B�[���h�Ɏw�肵���ԍ��̃G���g���ɕۑ�����āA����̃p���[�I�����ɁuStation List�v�ɕ\�������B<br>
</p>
</p>
<p><strong>���ӎ���</strong><br>
<p>�E����󋵁A�C���^�[�l�b�g���W�I�ǂ̃f�[�^�X�g���[���`���ɂ���Ă͉����r�؂�邱�Ƃ�����܂��B<br>
�E���܂ɉ�����ؒf����邱�ƁiLED����1�b�Ԋu�œ_�Łj������܂����A���̏ꍇ�̓C���^�[�l�b�g���W�I�ǂ̐؂�ւ����s���Ă݂Ă��������i�����������ꍇ��RESET�ł��j�B<br>
�E�N������"WiFi Err"���\�����ꂽ�ꍇ�A5���Ԋu�Ń��g���C���܂����A�ڑ��ł��Ȃ��ꍇ��WiFi�@��Ȃǂ̖��������K�v�ɂȂ邩������܂���B<br>
�E���p�̍ۂ́A���ȐӔC�ł��y���݂��������B</p>
</P>
