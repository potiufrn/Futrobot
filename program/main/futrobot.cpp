#include <iostream>
#include <fstream>
#include "futrobot.h"
#include <sys/time.h>
#include <system_tools.h>

#include <vssref_common.pb.h>
#include <vssref_placement.pb.h>
#include <vssref_command.pb.h>

inline void management2(void *x)
{
  ((Futrobot *)x)->management();
  std::cout << "Saiu do magnagement2\n";
}

inline void referee_communication2(void *x)
{
  ((Futrobot *)x)->referee_comunication();
  std::cout << "Saiu referee_communication2\n";
}

Futrobot::Futrobot(TEAM team, SIDE side, GAME_MODE gameMode)
    : FutData(team, side, gameMode),
      Acquisition(team, side, gameMode),
      Localization(team, side, gameMode),
      Strategy(team, side, gameMode),
      Obstacles(team, side, gameMode),
      Control(team, side, gameMode),
      Transmission(team, side, gameMode),
      Export(team, side, gameMode)
{
  t_start = t_end_acq = t_end_loc = t_end_str = t_end_obs =
      t_end_con = t_end_tra = t_end_exp = 0.0;
}

bool Futrobot::start_management()
{
  thr_ger = std::thread(management2, static_cast<void *>(this));

  if (thr_ger.joinable() == false)
    return true;

  if (sock_referee.connected())
  {
    thr_referee_comm = std::thread(referee_communication2, static_cast<void *>(this));
    if (thr_referee_comm.joinable() == false)
      return true;
  }
  return false;
}

bool Futrobot::finish_management()
{
  if (thr_ger.joinable())
    thr_ger.join();
  if (thr_referee_comm.joinable())
    thr_referee_comm.join();

  if (sock_cmd.connected())
    sock_cmd.close();
  if (sock_referee.connected())
    sock_referee.close();
  if (sock_vision.connected())
    sock_vision.close();
  if (sock_replacer.connected())
    sock_replacer.close();

  return (false);
}

//Laco principal do programa
void Futrobot::management()
{
  initialize(); // inicializado do export
  double myt_start, myt_end_cap, myt_end_acq, myt_end_loc, myt_end_str, myt_end_obs, myt_end_con, myt_end_tra, myt_end_exp;
  std::cout << "management!" << std::endl;
  if (gameState() == FINISH_STATE)
    std::cout << "gameState() == FINISH_STATE" << std::endl;

  while (gameState() != FINISH_STATE)
  {
    // Espera por uma nova imagem
    if (gameState() != FINISH_STATE && acquisitionWait())
    {
      finish();
      std::cerr << "Erro na espera por nova imagem!\n";
    }
    myt_start = relogio();
    dt_amostr = myt_start - t_start;
    //teste para evitar divisoes por zero.
    if (dt_amostr == 0.0)
      dt_amostr = 1.0 / FPS;
    // Lẽ a nova imagem

    if (gameState() != FINISH_STATE && acquisitionCapture())
    {
      finish();
      std::cerr << "Erro na leitura da nova imagem!\n";
    }

    id_pos++;
    myt_end_cap = relogio();
    // Fornece a pose dos robos e a posicao da
    // bola em coordenadas de mundo.
    if (gameState() != FINISH_STATE && acquisition())
    {
      finish();
      std::cerr << "Erro no processamento da imagem!\n";
    }

    myt_end_acq = relogio();
    //Realiza a correcao e filtragem da pose dos robos e da bola.
    //bem como a predição da posição da bola e calculo de sua velocidade e orientação
    if (gameState() != FINISH_STATE && localization())
    {
      finish();
      std::cerr << "Erro na localizacao dos robos!\n";
    }
    myt_end_loc = relogio();

    // Calcula as posicoes finais dos robos.
    if (gameState() != FINISH_STATE && strategy())
    {
      finish();
      std::cerr << "Erro na estrategia do jogo!\n";
    }
    myt_end_str = relogio();
    // Altera as posicoes finais desejadas em funcao de possiveis
    // obstaculos no caminho
    if (gameState() != FINISH_STATE && obstacles())
    {
      finish();
      std::cerr << "Erro no desvio de obstaculos!\n";
    }
    myt_end_obs = relogio();

    // Calcula o novo controle (ou seja, as novas tensoes dos motores)
    if (gameState() != FINISH_STATE && control())
    {
      finish();
      std::cerr << "Erro no controle dos robos!\n";
    }
    myt_end_con = relogio();

    // Informa as tensoes para os robos.
    if (gameState() != FINISH_STATE && transmission())
    {
      finish();
      std::cerr << "Erro na transmissao dos dados!\n";
    }
    myt_end_tra = relogio();

    // Modulo de exportacao dos dados
    if (gameState() != FINISH_STATE && exxport())
    {
      finish();
      std::cerr << "Erro na exportacao dos dados!\n";
    }

    myt_end_exp = relogio();

    t_start = myt_start;
    t_end_cap = myt_end_cap;
    t_end_acq = myt_end_acq;
    t_end_loc = myt_end_loc;
    t_end_str = myt_end_str;
    t_end_obs = myt_end_obs;
    t_end_con = myt_end_con;
    t_end_tra = myt_end_tra;
    t_end_exp = myt_end_exp;
  }
  // para os robos em caso de fim de jogo
  for (int i = 0; i < 3; i++)
  {
    pwm.me[i].right = 0.0;
    pwm.me[i].left = 0.0;
  }
  transmission();
}

void Futrobot::referee_comunication()
{
  std::cerr << "referee_comunication on!\n";
  VSSRef::ref_to_team::VSSRef_Command command;
  VSSRef::team_to_ref::VSSRef_Placement placementCommand;
  VSSRef::Frame *placementFrame = new VSSRef::Frame();
  uint8_t buffer[MAX_DGRAM_SIZE];
  uint8_t msg_to_replacer[MAX_DGRAM_SIZE];
  size_t packet_size;
  VSSRef::Color my_team = static_cast<VSSRef::Color>(myTeam());

  if (myTeam() == TEAM::BLUE_TEAM)
    placementFrame->set_teamcolor(VSSRef::Color::BLUE);
  else
    placementFrame->set_teamcolor(VSSRef::Color::YELLOW);
  placementCommand.set_allocated_world(placementFrame);

  while (thr_ger.joinable())
  { 
    try{
      memset(buffer, 0, MAX_DGRAM_SIZE);
      packet_size = sock_referee.recvFrom(buffer, MAX_DGRAM_SIZE, _multicast_address.c_str(), _referee_port, true);
    }catch(...)
    {
      std::cerr << "Erro no recebimento de pacotes do referee!\n";
      continue;
    }

    if (packet_size <= 0)
    {
      std::this_thread::yield();
      continue;
    }

    if (command.ParseFromArray(buffer, int(packet_size)) == false)
      continue;

    VSSRef::Foul foul = command.foul();

    switch (foul)
    {
    case VSSRef::Foul::GAME_ON:
      this->setGameState(GAME_STATE::PLAY_STATE);
      continue;
      break;
    case VSSRef::Foul::STOP:
      this->setGameState(GAME_STATE::PAUSE_STATE);
      continue;
      break;
    case VSSRef::Foul::KICKOFF:
      this->setAdvantage(my_team == command.teamcolor());
      this->setGameState(GAME_STATE::INICIALPOSITION_STATE);
      break;
    case VSSRef::Foul::FREE_BALL:
      continue; //deixa o referee posicionar
      // this->setAdvantage( my_team == command.teamcolor() );
      // this->setGameState(GAME_STATE::FREEBALL_STATE);
    case VSSRef::Foul::PENALTY_KICK:
      this->setAdvantage(my_team == command.teamcolor());
      this->setGameState(GAME_STATE::PENALTY_STATE);
      break;
    case VSSRef::Foul::FREE_KICK:
      this->setAdvantage(my_team == command.teamcolor());
      this->setGameState(GAME_STATE::FREEKICK_STATE);
      break;
    case VSSRef::Foul::GOAL_KICK:
      continue;
      // this->setAdvantage(my_team == command.teamcolor());
      // this->setGameState(GAME_STATE::GOALKICK_STATE);
      // break;
    default:
      continue;
      break;
    }

    try
    {
      //tempo para atualizar as referencias de posicao para passar pro referee
      usleep(0.5 * 1000000);
      //para garantir envia as referencias de posicao para o referee
      placementFrame->clear_robots();
      for (int i = 0; i < 3; i++)
      {
        VSSRef::Robot *robot = placementFrame->add_robots();
        robot->set_robot_id(static_cast<uint32_t>(i));
        robot->set_x(ref.me[i].x());
        robot->set_y(ref.me[i].y());
        robot->set_orientation( ref.me[i].theta() * (180.0/M_PI) );
      }
      memset(msg_to_replacer, 0, MAX_DGRAM_SIZE);
      placementCommand.SerializeToArray(msg_to_replacer, MAX_DGRAM_SIZE);
    }
    catch (...)
    {
      std::cerr << "Problema na comunicação com o referee!\n";
      continue;
    }

    if (sock_replacer.sendTo(msg_to_replacer, placementCommand.ByteSize(), _multicast_address.c_str(), _replacer_port) != SOCKET_OK)
      std::cerr << "Falha ao enviar mensagem para o replacer!\n";
  }
  delete placementFrame;
  std::cerr << "referee_comunication off!\n";
}

static double format(double x)
{
  if (x == POSITION_UNDEFINED || fabs(x) > 9.999)
    return 9.999;
  else
    return x;
}

void Futrobot::print_state() const
{
  printf("+--------+--------------------------+--------------------------+--------+\n");
  printf("| EQUIPE |           MEUS           |        ADVERSARIOS       |  BOLA  |\n");
  printf("| ROBO   | 0 CIAN | 1 ROSA | 2 VERD | 0 CIAN | 1 ROSA | 2 VERD |        |\n");
  printf("+--------+--------------------------+--------------------------+--------+\n");
  printf("| Pos x  | %+6.3f | %+6.3f | %+6.3f | %+6.3f | %+6.3f | %+6.3f | %+6.3f |\n",
         format(pos.me[0].x()), format(pos.me[1].x()), format(pos.me[2].x()),
         format(pos.op[0].x()), format(pos.op[1].x()), format(pos.op[2].x()),
         format(pos.ball.x()));
  printf("| Pos y  | %+6.3f | %+6.3f | %+6.3f | %+6.3f | %+6.3f | %+6.3f | %+6.3f |\n",
         format(pos.me[0].y()), format(pos.me[1].y()), format(pos.me[2].y()),
         format(pos.op[0].y()), format(pos.op[1].y()), format(pos.op[2].y()),
         format(pos.ball.y()));
  printf("| Pos th | %+6.3f | %+6.3f | %+6.3f | %+6.3f | %+6.3f | %+6.3f | ------ |\n",
         format(pos.me[0].theta()), format(pos.me[1].theta()), format(pos.me[2].theta()),
         format(pos.op[0].theta()), format(pos.op[1].theta()), format(pos.op[2].theta()));
  printf("+--------+--------------------------+--------------------------+--------+\n");
  printf("| Ref x  | %+6.3f | %+6.3f | %+6.3f | ------ | ------ | ------ | ------ |\n",
         format(ref.me[0].x()), format(ref.me[1].x()), format(ref.me[2].x()));
  printf("| Ref y  | %+6.3f | %+6.3f | %+6.3f | ------ | ------ | ------ | ------ |\n",
         format(ref.me[0].y()), format(ref.me[1].y()), format(ref.me[2].y()));
  printf("| Ref th | %+6.3f | %+6.3f | %+6.3f | ------ | ------ | ------ | ------ |\n",
         format(ref.me[0].theta()), format(ref.me[1].theta()), format(ref.me[2].theta()));
  printf("+--------+--------------------------+--------------------------+--------+\n");
  printf("| PWM di | %+6.3f | %+6.3f | %+6.3f | ------ | ------ | ------ | ------ |\n",
         format(pwm.me[0].right), format(pwm.me[1].right), format(pwm.me[2].right));
  printf("| PWM es | %+6.3f | %+6.3f | %+6.3f | ------ | ------ | ------ | ------ |\n",
         format(pwm.me[0].left), format(pwm.me[1].left), format(pwm.me[2].left));
  printf("+--------+--------------------------+--------------------------+--------+\n");
  printf("| Tempo de Amostragem:  %f    | Captura:     %f       |\n",
         dt_amostr,
         t_end_cap - t_start);
  printf("| Aquisicao:  %f              | Obstaculos:  %f       |\n",
         t_end_acq - t_end_cap,
         t_end_obs - t_end_str);
  printf("| Localizacao: %f             | Controle:    %f       |\n",
         t_end_loc - t_end_acq,
         t_end_con - t_end_obs);
  printf("| Estrategia:  %f             | Transmissao: %f       |\n",
         t_end_str - t_end_loc,
         t_end_tra - t_end_con);
  printf("| Exportacao:  %f             |\n",
         t_end_exp - t_end_tra);
  printf("+--------+--------------------------+--------------------------+--------+\n");

  std::cout << "Opções disponíveis:" << std::endl;
  std::cout << "P - Para im(P)rimir posições atuais" << std::endl;
  std::cout << "T - Para (T)erminar a partida" << std::endl;
  std::cout << "S - Para (S)alvar a configuracao atual" << std::endl;
  std::cout << "M - Para (M)enu de acoes." << std::endl;
  std::cout << "--> " << std::endl;
}