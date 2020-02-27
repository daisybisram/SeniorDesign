#include "spicontrol.h"
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <libgen.h>
#include <limits.h>
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "daccontrol.h"
#include "gpiocontrol.h"
#include "adccontrol.h"

#include <libdigiapix/spi.h>

#define DEFAULT_SPI_ALIAS		"DEFAULT_SPI"
#define DEFAULT_SPI_ADDRESS_SIZE	1
#define DEFAULT_SPI_PAGE_SIZE		16
#define DEFAULT_SPI_PAGE_INDEX		0

#define ARG_SPI_DEVICE		0
#define ARG_SPI_SLAVE		0

#define BUFF_SIZE			256

#define CLK_MODE			SPI_CLK_MODE_3
#define CHIP_SELECT			SPI_CS_ACTIVE_LOW
#define BIT_ORDER			SPI_BO_MSB_FIRST
#define MAX_BUS_SPEED		50000000
#define BITS_PER_WORD		SPI_BPW_8

#define OPERATION_BYTES		1

#define USER_LED0   34
#define USER_LED2   36

static spi_t *spi_dev;
static unsigned int page_size, address_bytes = 0;
static uint8_t *tx_buffer;
static uint8_t *rx_buffer;
int timer_flag = 0;
qint64 nanoSec;

SpiControl::SpiControl(QWidget *parent) :
    QScrollArea(parent)
    //ui(new Ui::SpiControl)
{
    //connect(timer, &QTimer::timeout, [this]() { SpiControl::timer_interrupt(USER_LED2); });

    int spi_device = 0, spi_slave = 1;
    spi_transfer_cfg_t transfer_mode;

    spi_device = ldx_spi_get_device(DEFAULT_SPI_ALIAS);
    spi_slave = ldx_spi_get_slave(DEFAULT_SPI_ALIAS);

    /* Request SPI */
    spi_dev = ldx_spi_request((unsigned int)spi_device, (unsigned int)spi_slave);

    /* Configure the transfer mode */
    transfer_mode.clk_mode = CLK_MODE;
    transfer_mode.chip_select = CHIP_SELECT;
    transfer_mode.bit_order = BIT_ORDER;

    ldx_spi_set_transfer_mode(spi_dev, &transfer_mode);

    /* Configure the bits-per-word */
    ldx_spi_set_bits_per_word(spi_dev, BITS_PER_WORD);

    /* Configure the max bus speed */
    ldx_spi_set_speed(spi_dev, MAX_BUS_SPEED);

    /* Initialize the write and read buffers */
    tx_buffer = (uint8_t *)calloc(page_size, sizeof(uint8_t));
    rx_buffer = (uint8_t *)calloc(page_size, sizeof(uint8_t));
}

SpiControl::~SpiControl()
{
    /* Free spi */
    ldx_spi_free(spi_dev);

    /* Free buffers */
    free(tx_buffer);
    free(rx_buffer);
}

//int SpiControl::spi_set_transfer(spi_clk_mode_t clock_mode)
//{
//    /* Configure the transfer mode */
//    spi_transfer_cfg_t transfer_mode;
//    transfer_mode.clk_mode = clock_mode;
//    transfer_mode.chip_select = CHIP_SELECT;
//    transfer_mode.bit_order = BIT_ORDER;

//    ldx_spi_set_transfer_mode(spi_dev, &transfer_mode);
//}

int SpiControl::spi_Read_DAC(uint8_t reg, uint16_t *data)
{
    uint8_t read_reg = DAC_READ_BIT | reg;

    uint8_t tx_data8[3] = {read_reg, 0x00, 0x00};
    uint8_t rx_data8[3] = {0x00, 0x00, 0x00};


//READ ACCESS CYCLE
  if (ldx_spi_write(spi_dev, tx_data8, 3) != EXIT_SUCCESS)
            {
                return EXIT_FAILURE;
            }


//READ CYCLE
//After the READ ACCESS cycle, we send any command on MOSI, but the data we'll get back will be from the previous READ ACCESS command
//On MISO, we will see the first byte echoing our command from the READ ACCESS cycle
//so we know on MOSI we won't be trying to read DEVID again.

    if (ldx_spi_read(spi_dev, rx_data8, 3) != EXIT_SUCCESS)
            {
                return EXIT_FAILURE;
            }

   *data = rx_data8[1] << 8 | rx_data8[2];
        return EXIT_SUCCESS;

}

int SpiControl::spi_Write_DAC(uint8_t reg, uint16_t data)
{
    uint8_t write_reg = DAC_WRITE_BIT | reg;
    uint8_t write_data_8MSB = data>>8;
    uint8_t write_data_8LSB = data;

    uint8_t tx_data8[3] = {write_reg, write_data_8MSB, write_data_8LSB};
    uint8_t rx_data8[3] = {0x00, 0x00, 0x00};



    if (ldx_spi_write(spi_dev, tx_data8, 3) != EXIT_SUCCESS)
        {
            return EXIT_FAILURE;
        }

    return EXIT_SUCCESS;

}


void SpiControl::timer_interrupt(int gpionum)
{
    GpioControl *gpiocontrol;
    gpiocontrol->ToggleGpio(gpionum);
    //timer_flag = 1;
}

int SpiControl::DAC_output(uint8_t output, float_t gain, uint16_t delay_time_ms, uint32_t amplitude_uV)
{
    uint16_t latency_array_size = 0;
    uint16_t count;
    uint16_t i = 0;
    uint16_t output_data_size = 0;
    uint32_t scalar = amplitude_uV/gain;
    float_t raw_EMG_data[400] = {-0.005917892881671365,-0.005754401201136526,-0.004058393878576869,-0.0011026109840860919,0.002443067879585488,0.005841658689641595,0.00863740629478728,0.010746657766923452,0.01238136701206603,0.013794977371827465,0.01510148255528397,0.01623843365615453,0.016992795811311745,0.017178851957785957,0.016725098632198794,0.0157742447788333,0.014640148144555613,0.013687613259028151,0.013097423006466425,0.01274341366255807,0.012181126899843775,0.010935917738752807,0.00882894531877754,0.006156745357330308,0.0036091719416414684,0.0019401640917555852,0.001654564331127989,0.002955901881975102,0.005842361517269214,0.010247232975850267,0.015862114672698106,0.021785827460338535,0.02626896350187359,0.026843679216318427,0.021138321820249575,0.007870756986455977,-0.012554075598770094,-0.038113643528293335,-0.06577919950810288,-0.09225385311889009,-0.11403840644727212,-0.1272366050712918,-0.127412517483351,-0.11001747204821644,-0.07175935839646795,-0.011987799981011323,0.06688315264635153,0.1601959121736732,0.2631142086674676,0.3724361690715565,0.4869035675283772,0.6053922908440467,0.7241405671695652,0.8347855697568751,0.9251433090193335,0.982743526690393,1.0,0.9779799172143037,0.9263278407285733,0.8591730719198168,0.788684598498415,0.7197970102288136,0.6491724923952817,0.5684985670125593,0.4705611811449843,0.35428615935116264,0.22603409166105298,0.09682815124686142,-0.022599360714566082,-0.1259373603849358,-0.212550898489085,-0.2858872623384267,-0.35046376348165187,-0.4095843082840361,-0.46471661455740976,-0.5162396338062555,-0.5646722665451173,-0.6112045931224002,-0.6574919575557874,-0.7050594135951525,-0.7547320644349081,-0.8062894030445178,-0.8586455401887936,-0.9098722210051751,-0.9575175634980101,-0.9989506906822729,-1.031783632665291,-1.0544173538416517,-1.06638864559362,-1.0683407390540762,-1.0618858221583054,-1.0491181979335225,-1.0321605291182308,-1.012638772493053,-0.991281649335047,-0.967667257562487,-0.9403930244974474,-0.9076611910503328,-0.8681460434709164,-0.8215126083577302,-0.7682316980192181,-0.7090096336706221,-0.6438100408996358,-0.5715735406359727,-0.49093434562432264,-0.4014559629170059,-0.30497026545922795,-0.20569421901670715,-0.10890034602957877,-0.01867878886770359,0.06400388158135394,0.14177880740426851,0.2192828363328181,0.3003486600447426,0.3854339590691274,0.47079114141261774,0.5498783614991848,0.6162548822887027,0.6664038940760674,0.7009867124477349,0.7238933444689727,0.7399539832858548,0.7524698636966515,0.761969544958157,0.7666673927848784,0.7639974698205406,0.7524016359854322,0.7323248099385581,0.70612413579945,0.6769660369851737,0.6475083116559647,0.6189578915828787,0.5909148637356973,0.5619873417045173,0.530869751244231,0.4971839628709728,0.461736378810117,0.42610630617226514,0.3917992757176343,0.35955633903253925,0.3292040497667722,0.3000447179215702,0.2714410720135272,0.2433144035685559,0.2161217560497635,0.19053611773636894,0.16695931285652577,0.14519196030955653,0.12444389277727476,0.10367083581823315,0.08210212455338592,0.059681491671492615,0.03730200092148511,0.01650026160806139,-0.0011560509554140127,-0.014894092918744495,-0.02513235559461067,-0.03332899032440633,-0.04121525061970079,-0.04985363922912551,-0.059067586124127575,-0.0676732172119609,-0.074253821060086,-0.07816004248202549,-0.07989923260265291,-0.08082184799276951,-0.08224739614694276,-0.08460523898399896,-0.08720440652372002,-0.08879544797863075,-0.08846448192767123,-0.08636343833139679,-0.08366538171143871,-0.08179551086320795,-0.08146072241637893,-0.08215043881515592,-0.08252022478975796,-0.0813136629279799,-0.07830500581785092,-0.07445942896693793,-0.07124549275820387,-0.06958626469310478,-0.0692105396195442,-0.06883662299134125,-0.0670950079098934,-0.0634340979906117,-0.05843249299535681,-0.053278588130268895,-0.048931064940039765,-0.04557771403464571,-0.0426864620191539,-0.039608385267910905,-0.036122278032855344,-0.032545789630949735,-0.02946880205702439,-0.02729807166293538,-0.025965802298287443,-0.024989180153627433,-0.02369885439096698,-0.021536755213357506,-0.01831211424113728,-0.01432111783295637,-0.010324722804372657,-0.00729184280170931,-0.005935436856691803,-0.006235649061314107,-0.007303377806018753,-0.00783686302136144,-0.006852489222282185,-0.004382530993259842,-0.0014386328728510842,0.0006321494729409308,0.0010656665004814985,0.0001786473774048727,-0.0009209468937690426,-0.0011900281500960325,-0.0004078107988026448,0.0006860962199792933,0.0010364837410487093,0.00011221232874741731,-0.001579081876952556,-0.002742906064827012,-0.0021347156158023598,0.0006777738777466278,0.004944049166697151,0.009109233798692905,0.011586399093475664,0.011511743181030725,0.00908692621419638,0.005383320544062566,0.0018087534098442724,-0.0005168302556196413,-0.0011770237839335248,-0.00044849465419442185,0.0009491000723789154,0.0022667033561868766,0.0030245508787605903,0.0031047554303709492,0.0026016068859023053,0.0017191500800031565,0.0006142501795087523,-0.0005382664982620429,-0.0015566674297758064,-0.002262116481143464,-0.0025941388285959602,-0.0026972284341863577,-0.002916724379364159,-0.0035949407097079404,-0.004833592201326001,-0.006403086440811022,-0.007793293873603644,-0.008438962297613387,-0.008047125619957674,-0.006674608070845025,-0.004723657219046711,-0.0027446014823497788,-0.0012199575097543022,-0.000415479223634408,-0.0003679082740074204,-0.000914764887718149,-0.0018193258280110923,-0.0028041466008008947,-0.0036397798793191536,-0.004120400948776196,-0.004075259686588199,-0.0034546402858330093,-0.002372799502184314,-0.0012098933058340036,-0.0005100150881884269,-0.0007781588300016153,-0.00215814731349277,-0.004272842617165681,-0.006321519390027821,-0.00746529737212339,-0.007296789310772704,-0.006110447101123744,-0.004695981839920527,-0.003888987759702001,-0.004045444998806015,-0.0048839224686594365,-0.005725073560569973,-0.006019702190150631,-0.005697618071958863,-0.00521755803157299,-0.005171755042788228,-0.005872780390204967,-0.007119514399540655,-0.008361490964554881,-0.009129808974738897,-0.009337989806122328,-0.00932548687253625,-0.009472003827327992,-0.009860258550144697,-0.010041244884462949,-0.009306703701435536,-0.0072493278953929445,-0.004183809564127678,-0.0011479137733253486,0.0006318952630650929,0.0005006974844936086,-0.001172470611735413,-0.003163268912947606,-0.004154126399644229,-0.0036134197331803277,-0.0021063291888423854,-0.0009057113581465737,-0.00109344319364874,-0.0027611405371658127,-0.004953056457609013,-0.006331402132896896,-0.006099429969803072,-0.004577960229820524,-0.003015015439602241,-0.0027570407093380307,-0.004355435138668713,-0.0071720661775918,-0.009698577370019017,-0.010427477436561586,-0.008685095820888285,-0.004987938902495614,-0.0008193067160317449,0.0021679971755371595,0.0029319708067598047,0.0015331644217968631,-0.0010139203026351105,-0.0033229258673036777,-0.004395681268081885,-0.00407321080020008,-0.003012294551359643,-0.002152187664018801,-0.002108848682033794,-0.0028310266749800353,-0.0036834579448087137,-0.003928035794065751,-0.0031712178523149543,-0.00163973692874191,-2.724681867745925e-05,0.0008345436901902937,0.0003717386845779479,-0.0014700669412489535,-0.004203451746773261,-0.0069614090090993576,-0.008829843376301721,-0.009223973491479139,-0.008112981801696569,-0.006070361265730907,-0.00398603551375672,-0.0026261647518381406,-0.0022806797617291037,-0.0026910756098673713,-0.0032717859096624007,-0.0034967092459645983,-0.0032123928650254477,-0.0026332916705883898,-0.0021813673408742768,-0.002157495862153719,-0.002573709561209505,-0.00318873100156225,-0.0036541487997060456,-0.003729265043696561,-0.003367415678152624,-0.0026661581099115548,-0.0017624709775345875,-0.000776148167572191,0.0001772823094176025,0.0009180570802896307,0.0012024848450223982,0.0008051428897888188,-0.0002590928838086188,-0.0016402434989471971,-0.002731843722372401,-0.003026139433603134,-0.0024061406584138536,-0.0013273785474813495,-0.000485313368644599,-0.00039432883866160245,-0.0009934004074756198,-0.0016689159890819222,-0.0017396648375337903,-0.0009550420484096162,0.00023529908603637687,0.0009531335220418663,0.0005054189654130715,-0.0011426834616439015,-0.0032965020145670275,-0.004938537518459501,-0.005366208129866106,-0.004569409160351152,-0.003198114366906437,-0.0020982651665886433,-0.0017778213905382963,-0.002097663036486618,-0.002423824376939198,-0.002086820584547081,-0.0009177371087644249,0.0005331958640854672,0.0012363079108387118,0.0003169746435410347,-0.002284818635536198,-0.005614813880146928
                                 };
    //This is same raw data just scaled by a factor of 100
    //float_t raw_EMG_data[400] = {-0.5917892881671365,-0.5754401201136526,-0.40583938785768686,-0.11026109840860919,0.2443067879585488,0.5841658689641596,0.863740629478728,1.074665776692345,1.238136701206603,1.3794977371827464,1.510148255528397,1.623843365615453,1.6992795811311745,1.7178851957785957,1.6725098632198794,1.57742447788333,1.4640148144555614,1.368761325902815,1.3097423006466424,1.274341366255807,1.2181126899843775,1.0935917738752807,0.882894531877754,0.6156745357330308,0.36091719416414686,0.1940164091755585,0.1654564331127989,0.2955901881975102,0.5842361517269214,1.0247232975850267,1.5862114672698107,2.1785827460338534,2.626896350187359,2.684367921631843,2.1138321820249573,0.7870756986455977,-1.2554075598770094,-3.8113643528293335,-6.577919950810288,-9.225385311889008,-11.403840644727211,-12.72366050712918,-12.7412517483351,-11.001747204821644,-7.175935839646795,-1.1987799981011322,6.688315264635153,16.01959121736732,26.311420866746758,37.243616907155655,48.69035675283772,60.53922908440467,72.41405671695652,83.47855697568751,92.51433090193335,98.2743526690393,100.0,97.79799172143036,92.63278407285733,85.91730719198168,78.8684598498415,71.97970102288136,64.91724923952818,56.84985670125593,47.05611811449843,35.42861593511626,22.603409166105298,9.682815124686142,-2.2599360714566084,-12.59373603849358,-21.2550898489085,-28.58872623384267,-35.04637634816519,-40.95843082840361,-46.47166145574098,-51.62396338062555,-56.46722665451173,-61.120459312240015,-65.74919575557874,-70.50594135951525,-75.47320644349081,-80.62894030445177,-85.86455401887936,-90.9872221005175,-95.75175634980101,-99.89506906822729,-103.17836326652909,-105.44173538416517,-106.638864559362,-106.83407390540762,-106.18858221583054,-104.91181979335225,-103.21605291182307,-101.2638772493053,-99.1281649335047,-96.76672575624869,-94.03930244974474,-90.76611910503328,-86.81460434709165,-82.15126083577302,-76.82316980192181,-70.9009633670622,-64.38100408996358,-57.15735406359727,-49.093434562432265,-40.14559629170059,-30.497026545922797,-20.569421901670715,-10.890034602957876,-1.867878886770359,6.400388158135393,14.17788074042685,21.928283633281808,30.03486600447426,38.54339590691274,47.079114141261776,54.98783614991848,61.625488228870275,66.64038940760673,70.0986712447735,72.38933444689727,73.99539832858548,75.24698636966515,76.1969544958157,76.66673927848784,76.39974698205407,75.24016359854322,73.2324809938558,70.612413579945,67.69660369851736,64.75083116559647,61.895789158287876,59.09148637356974,56.19873417045172,53.0869751244231,49.71839628709728,46.1736378810117,42.61063061722651,39.17992757176343,35.955633903253926,32.92040497667722,30.004471792157023,27.144107201352718,24.33144035685559,21.61217560497635,19.053611773636895,16.695931285652577,14.519196030955653,12.444389277727476,10.367083581823316,8.210212455338592,5.968149167149262,3.730200092148511,1.650026160806139,-0.11560509554140128,-1.4894092918744495,-2.5132355594610667,-3.332899032440633,-4.121525061970079,-4.985363922912551,-5.906758612412758,-6.76732172119609,-7.4253821060086,-7.816004248202549,-7.989923260265291,-8.08218479927695,-8.224739614694276,-8.460523898399897,-8.720440652372002,-8.879544797863074,-8.846448192767124,-8.636343833139678,-8.36653817114387,-8.179551086320796,-8.146072241637894,-8.215043881515593,-8.252022478975796,-8.13136629279799,-7.830500581785092,-7.445942896693793,-7.124549275820387,-6.958626469310477,-6.92105396195442,-6.883662299134125,-6.709500790989339,-6.3434097990611695,-5.843249299535682,-5.327858813026889,-4.893106494003977,-4.557771403464571,-4.268646201915391,-3.9608385267910906,-3.6122278032855344,-3.2545789630949735,-2.9468802057024392,-2.729807166293538,-2.5965802298287444,-2.4989180153627433,-2.369885439096698,-2.153675521335751,-1.831211424113728,-1.432111783295637,-1.0324722804372657,-0.729184280170931,-0.5935436856691803,-0.6235649061314107,-0.7303377806018754,-0.783686302136144,-0.6852489222282184,-0.4382530993259842,-0.14386328728510842,0.06321494729409308,0.10656665004814986,0.01786473774048727,-0.09209468937690425,-0.11900281500960325,-0.04078107988026448,0.06860962199792933,0.10364837410487093,0.011221232874741732,-0.15790818769525558,-0.2742906064827012,-0.21347156158023597,0.06777738777466277,0.4944049166697151,0.9109233798692905,1.1586399093475663,1.1511743181030725,0.908692621419638,0.5383320544062566,0.18087534098442723,-0.05168302556196413,-0.11770237839335247,-0.04484946541944219,0.09491000723789154,0.22667033561868766,0.302455087876059,0.3104755430370949,0.2601606885902305,0.17191500800031564,0.061425017950875224,-0.05382664982620429,-0.15566674297758065,-0.22621164811434638,-0.25941388285959605,-0.2697228434186358,-0.2916724379364159,-0.35949407097079406,-0.48335922013260013,-0.6403086440811022,-0.7793293873603644,-0.8438962297613386,-0.8047125619957675,-0.6674608070845025,-0.47236572190467113,-0.2744601482349779,-0.12199575097543022,-0.0415479223634408,-0.03679082740074204,-0.0914764887718149,-0.18193258280110924,-0.2804146600800895,-0.3639779879319154,-0.4120400948776196,-0.4075259686588199,-0.34546402858330094,-0.23727995021843143,-0.12098933058340036,-0.05100150881884269,-0.07781588300016154,-0.215814731349277,-0.42728426171656814,-0.6321519390027821,-0.7465297372123391,-0.7296789310772704,-0.6110447101123744,-0.4695981839920527,-0.3888987759702001,-0.4045444998806015,-0.4883922468659436,-0.5725073560569973,-0.6019702190150631,-0.5697618071958863,-0.521755803157299,-0.5171755042788228,-0.5872780390204967,-0.7119514399540655,-0.8361490964554882,-0.9129808974738897,-0.9337989806122329,-0.932548687253625,-0.9472003827327993,-0.9860258550144697,-1.0041244884462948,-0.9306703701435536,-0.7249327895392944,-0.4183809564127678,-0.11479137733253486,0.06318952630650929,0.05006974844936086,-0.1172470611735413,-0.3163268912947606,-0.41541263996442296,-0.36134197331803275,-0.21063291888423855,-0.09057113581465737,-0.109344319364874,-0.27611405371658126,-0.4953056457609013,-0.6331402132896896,-0.6099429969803072,-0.45779602298205246,-0.3015015439602241,-0.27570407093380306,-0.4355435138668713,-0.71720661775918,-0.9698577370019017,-1.0427477436561587,-0.8685095820888284,-0.4987938902495614,-0.08193067160317449,0.21679971755371596,0.2931970806759805,0.15331644217968632,-0.10139203026351105,-0.33229258673036777,-0.4395681268081885,-0.40732108002000794,-0.3012294551359643,-0.21521876640188012,-0.2108848682033794,-0.28310266749800356,-0.36834579448087135,-0.3928035794065751,-0.3171217852314954,-0.163973692874191,-0.002724681867745925,0.08345436901902936,0.037173868457794794,-0.14700669412489534,-0.42034517467732607,-0.6961409009099357,-0.8829843376301721,-0.9223973491479138,-0.8112981801696568,-0.6070361265730907,-0.398603551375672,-0.26261647518381404,-0.22806797617291036,-0.2691075609867371,-0.3271785909662401,-0.3496709245964598,-0.32123928650254474,-0.26332916705883896,-0.21813673408742768,-0.21574958621537188,-0.2573709561209505,-0.318873100156225,-0.36541487997060457,-0.3729265043696561,-0.3367415678152624,-0.2666158109911555,-0.17624709775345876,-0.0776148167572191,0.01772823094176025,0.09180570802896307,0.12024848450223982,0.08051428897888188,-0.02590928838086188,-0.1640243498947197,-0.27318437223724007,-0.3026139433603134,-0.24061406584138537,-0.13273785474813496,-0.0485313368644599,-0.03943288386616024,-0.09934004074756199,-0.16689159890819222,-0.17396648375337903,-0.09550420484096162,0.023529908603637687,0.09531335220418663,0.050541896541307155,-0.11426834616439015,-0.32965020145670276,-0.4938537518459501,-0.5366208129866106,-0.45694091603511516,-0.31981143669064366,-0.20982651665886431,-0.17778213905382964,-0.2097663036486618,-0.2423824376939198,-0.2086820584547081,-0.0917737108764425,0.05331958640854672,0.12363079108387118,0.03169746435410347,-0.2284818635536198,-0.5614813880146928
    // };

    latency_array_size = delay_time_ms * 3;
    output_data_size = latency_array_size + 400;
    uint16_t output_data[output_data_size];

    for(count=0; count<400; count++)
        {
            raw_EMG_data[count] = (((raw_EMG_data[count]*scalar)+625000)/1000000/0.00001907)/2;
        }

    for (count = 0; count<latency_array_size; count++)
        {
            //output_data[count] = 32768;       //use this to check latency
            output_data[count] = 0;
        }

    for(count; count<output_data_size; count++)
        {
            output_data[count] = (uint16_t)raw_EMG_data[i];
            //output_data[count] = 0x0000;
            i++;
        }

    //you had this on infinite loop before with i as an uint8_t

        for (i = 0; i<output_data_size; i++)
        {
            spi_Write_DAC(output, output_data[i]);
            us_timer->start();
            nanoSec = us_timer->nsecsElapsed();
            do
            {
                timer_flag = 1;
            }
            while(nanoSec<61);

            if(timer_flag == 1)
            {
                spi_Write_DAC(DAC_TRIGGER_ADDR, 0x0000);         //Pulls LDAC LOW to update register
                spi_Write_DAC(DAC_TRIGGER_ADDR, 0x0010);         //Set LDAC HIGH to reset synchronous mode
            }
            us_timer->restart();
        }


}

//ADC Communication Functions: Address should be 9 bits

int SpiControl::spi_Read_ADC(uint8_t reg, uint8_t *data)
{
    uint8_t command = ADS_READ_REG << 1;    //Command is left-bit shifted by 1 to inclue the "0" for the 9th bit of the address

    uint8_t tx_data8[3] = {command, reg, 0x00};
    uint8_t rx_data8[3] = {0x00, 0x00, 0x00};

//READ ACCESS CYCLE
//Send a read register command to a specified address. The contents will be pushed onto the SDO line for the "READ CYCLE"
    if (ldx_spi_write(spi_dev, tx_data8, 3) != EXIT_SUCCESS)
            {
                return EXIT_FAILURE;
            }

//READ CYCLE
//Read the data pushed onto the SDO line from the "READ ACCESS CYCLE"
    if (ldx_spi_read(spi_dev, rx_data8, 3) != EXIT_SUCCESS)
            {
                return EXIT_FAILURE;
            }

    *data = rx_data8[0];

    return EXIT_SUCCESS;

}

int SpiControl::spi_Check_ADC_Reg(uint8_t reg, uint32_t *data)
{
    uint8_t command = ADS_READ_REG << 1;

    uint8_t tx_data8[3] = {command, reg, 0x00};
    uint8_t rx_data8[3] = {0x00, 0x00, 0x00};

    if (ldx_spi_write(spi_dev, tx_data8, 3) != EXIT_SUCCESS)
            {
                return EXIT_FAILURE;
            }

    if (ldx_spi_read(spi_dev, rx_data8, 3) != EXIT_SUCCESS)
            {
                return EXIT_FAILURE;
            }

     *data =  0x00 << 24 | rx_data8[2] << 16 | rx_data8[1] << 8 | rx_data8[0];
}


int SpiControl::spi_Write_ADC(uint8_t reg, uint8_t data)
{
    uint8_t command = ADS_WRITE_REG << 1;         //Command is left-bit shifted by 1 to inclue the "0" for the 9th bit of the address

    uint8_t tx_data8[3] = {command, reg, data};

    if (ldx_spi_write(spi_dev, tx_data8, 3) != EXIT_SUCCESS)
        {
            return EXIT_FAILURE;
        }

    return EXIT_SUCCESS;
}


int SpiControl::spi_Read_ADC_Conversion(uint32_t *data)
{
    uint8_t rx_data8[3] = {0x00, 0x00, 0x00};

//READ CYCLE
//Read the data pushed onto the SDO line from the "SPI_WRITE" (Previously wrote to set DATA_VAL bit)
    if (ldx_spi_read(spi_dev, rx_data8, 3) != EXIT_SUCCESS)
            {
                return EXIT_FAILURE;
            }

    *data = rx_data8[0] << 12 | rx_data8[1] << 4 | rx_data8[2] >> 4;  //18-bit ADC CONVERSION
    return EXIT_SUCCESS;
}

