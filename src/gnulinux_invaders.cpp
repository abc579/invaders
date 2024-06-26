#include "common.h"
#include "event.h"
#include "invaders.h"
#include "invaders_opengl.h"
#include "invaders_timer.h"
#include "invaders_grid.h"
#include "invaders_input.h"
#include "invaders_level.h"
#include "invaders_menu.h"
#include "invaders_missile.h"
#include "invaders_player.h"
#include "invaders_text.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>

#include <GL/glx.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>


#define GLX_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB 0x2092

// --------------------------------------------------------
// OpenGL function pointers
// --------------------------------------------------------
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
PFNGLUNIFORM1IPROC glUniform1i;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
PFNGLDELETEBUFFERSPROC glDeleteBuffers;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLCREATESHADERPROC glCreateShader;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLDELETESHADERPROC glDeleteShader;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLGETPROGRAMIVPROC glGetProgramiv;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
PFNGLXSWAPINTERVALEXTPROC glXSwapIntervalEXT;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
PFNGLDRAWARRAYSINSTANCEDPROC glDrawArraysInstanced;
PFNGLVERTEXATTRIBDIVISORPROC glVertexAttribDivisor;
PFNGLBUFFERSUBDATAPROC glBufferSubData;
PFNGLDELETEPROGRAMPROC glDeleteProgram;
PFNGLUNIFORM1FPROC glUniform1f;
PFNGLUNIFORM3FPROC glUniform3f;
PFNGLUNIFORM4FPROC glUniform4f;
PFNGLPOLYGONMODEPROC myGlPolygonMode;

namespace Game {
  void initOpenGLfptrs()
  {
    glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)getGLProcAddress("glGetUniformLocation");
    glUniform1i = (PFNGLUNIFORM1IPROC)getGLProcAddress("glUniform1i");
    glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)getGLProcAddress("glGenVertexArrays");
    glGenBuffers = (PFNGLGENBUFFERSPROC)getGLProcAddress("glGenBuffers");
    glBindBuffer = (PFNGLBINDBUFFERPROC)getGLProcAddress("glBindBuffer");
    glBufferData = (PFNGLBUFFERDATAPROC)getGLProcAddress("glBufferData");
    glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)getGLProcAddress("glBindVertexArray");
    glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)getGLProcAddress("glEnableVertexAttribArray");
    glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)getGLProcAddress("glVertexAttribPointer");
    glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)getGLProcAddress("glDeleteBuffers");
    glGetShaderiv = (PFNGLGETSHADERIVPROC)getGLProcAddress("glGetShaderiv");
    glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)getGLProcAddress("glGetShaderInfoLog");
    glCreateShader = (PFNGLCREATESHADERPROC)getGLProcAddress("glCreateShader");
    glShaderSource = (PFNGLSHADERSOURCEPROC)getGLProcAddress("glShaderSource");
    glCompileShader = (PFNGLCOMPILESHADERPROC)getGLProcAddress("glCompileShader");
    glDeleteShader = (PFNGLDELETESHADERPROC)getGLProcAddress("glDeleteShader");
    glCreateProgram = (PFNGLCREATEPROGRAMPROC)getGLProcAddress("glCreateProgram");
    glAttachShader = (PFNGLATTACHSHADERPROC)getGLProcAddress("glAttachShader");
    glLinkProgram = (PFNGLLINKPROGRAMPROC)getGLProcAddress("glLinkProgram");
    glGetProgramiv = (PFNGLGETPROGRAMIVPROC)getGLProcAddress("glGetProgramiv");
    glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)getGLProcAddress("glGetProgramInfoLog");
    glUseProgram = (PFNGLUSEPROGRAMPROC)getGLProcAddress("glUseProgram");
    glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)getGLProcAddress("glGenerateMipmap");
    glXSwapIntervalEXT = (PFNGLXSWAPINTERVALEXTPROC) glXGetProcAddress(reinterpret_cast<const GLubyte*>("glXSwapIntervalEXT"));
    glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC) glXGetProcAddress(reinterpret_cast<const GLubyte*>("glDeleteVertexArrays"));
    glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC) glXGetProcAddress(reinterpret_cast<const GLubyte*>("glUniformMatrix4fv"));
    glDrawArraysInstanced = (PFNGLDRAWARRAYSINSTANCEDPROC)getGLProcAddress("glDrawArraysInstanced");
    glVertexAttribDivisor = (PFNGLVERTEXATTRIBDIVISORPROC)getGLProcAddress("glVertexAttribDivisor");
    glBufferSubData = (PFNGLBUFFERSUBDATAPROC)getGLProcAddress("glBufferSubData");
    glDeleteProgram = (PFNGLDELETEPROGRAMPROC)getGLProcAddress("glDeleteProgram");
    glUniform1f = (PFNGLUNIFORM1FPROC)getGLProcAddress("glUniform1f");
    glUniform3f = (PFNGLUNIFORM3FPROC)getGLProcAddress("glUniform3f");
    glUniform4f = (PFNGLUNIFORM4FPROC)getGLProcAddress("glUniform4f");
    myGlPolygonMode = (PFNGLPOLYGONMODEPROC)getGLProcAddress("glPolygonMode");
  }

  TexInfo loadTexFromFile(const LoadTexFromFileArgs& args)
  {
    if(args.m_flip) {
      stbi_set_flip_vertically_on_load(true);
    }
    int width, height, channels;
    unsigned char* data{ stbi_load(args.m_filepath, &width, &height, &channels, 0) };
    if(!data) {
      std::cerr << __FUNCTION__ << " failed loading image -> " << args.m_filepath << '\n';
      exit(EXIT_FAILURE);
    }
    // don't assume dimensions are multiple of 4
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    unsigned int id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 (args.m_alpha) ? GL_RGBA : GL_RGB,
                 width,
                 height,
                 0,
                 (args.m_alpha) ? GL_RGBA : GL_RGB
                 ,
                 GL_UNSIGNED_BYTE,
                 data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, args.m_wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, args.m_wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
    if(args.m_flip) {
      stbi_set_flip_vertically_on_load(false);
    }
    return Game::TexInfo {
      .m_width = width,
      .m_height = height,
      .m_channels = channels,
      .m_id = id
    };
  }

  static float volume{ MIX_MAX_VOLUME };

  std::unique_ptr<AudioData> openAudioFile(const char* filepath, const AudioType type)
  {
    AudioData data;
    data.m_type = type;
    switch(data.m_type) {
    case AudioType::MUSIC:
      data.m_data = Mix_LoadMUS(filepath);
      if(!data.m_data) {
	std::cerr << __FUNCTION__ << ": couldn't open file " << filepath << ':' << Mix_GetError() << '\n';
	return nullptr;
      }
      break;
    case AudioType::EFFECT:
      data.m_data = Mix_LoadWAV(filepath);
      if(!data.m_data) {
	std::cerr << __FUNCTION__ << ": couldn't open file " << filepath << ':' << Mix_GetError() << '\n';
	return nullptr;
      }
      break;
    }
    return std::make_unique<AudioData>(data);
  }

  bool initAudioSystem(const AudioDevice& audioDevice)
  {
    if(Mix_OpenAudioDevice(44100, MIX_DEFAULT_FORMAT, 2, 2048, audioDevice.m_name.c_str(), 0) < 0) {
      std::cerr << __FUNCTION__  << ": couldn't open audio device: " << Mix_GetError() << '\n';
      return false;
    }
    Mix_AllocateChannels(32);
    return true;
  }

  std::vector<AudioDevice> getAudioDevices()
  {
    std::vector<AudioDevice> res;
    for(int i{ 0 }; i < SDL_GetNumAudioDevices(0); ++i) {
      res.emplace_back(AudioDevice{ .m_name = std::string(SDL_GetAudioDeviceName(i, 0)), .m_index = i });
    }
    return res;
  }

  void playAudioTrack(AudioData* data, const bool loop)
  {
    switch(data->m_type) {
    case AudioType::MUSIC:
      // @NOTE: beware! only one music can be played at a time!
      // -1 means loop forevah
      Mix_PlayMusic(reinterpret_cast<Mix_Music*>(data->m_data), (loop) ? -1 : 0);
      break;
    case AudioType::EFFECT:
      // you can play as many effects as you want
      // -1: play on the first free channel you find
      //  0; -1: play once and stop; play in loop
      if(Mix_PlayChannel(-1, reinterpret_cast<Mix_Chunk*>(data->m_data), (loop) ? -1 : 0) == -1) {
	assert(false && "audio couldn't be played!!!!!!!!!!!!!!!!!!");
      }
      break;
    }
  }

  void stopAudioTrack(AudioData* data, const unsigned int delay)
  {
    // you only need to stop main music for now
    switch(data->m_type){
    case AudioType::MUSIC:
      Mix_FadeOutMusic(delay);
      break;
    default:
      break;
    }
  }

  void increaseVolume()
  {
    volume = std::min(volume + 13.3f, static_cast<float>(MIX_MAX_VOLUME));
    Mix_MasterVolume(static_cast<int>(volume));
    Mix_VolumeMusic(static_cast<int>(volume));
  }

  void decreaseVolume()
  {
    volume = std::max(volume - 13.3f, 0.0f);
    Mix_MasterVolume(static_cast<int>(volume));
    Mix_VolumeMusic(static_cast<int>(volume));
  }

  float getNormalizedVolumeValue()
  {
    return std::round((volume / MIX_MAX_VOLUME) * 10.f) / 10.f;
  }

  void closeAudioSystem()
  {
    Mix_CloseAudio();
    SDL_Quit();
  }

  void closeAudioFile(AudioData* data)
  {
    if(data->m_type == AudioType::MUSIC) {
      Mix_FreeMusic(reinterpret_cast<Mix_Music*>(data->m_data));
    } else {
      Mix_FreeChunk(reinterpret_cast<Mix_Chunk*>(data->m_data));
    }
  }

  // the file is always sorted and always contains max 5 lines
  bool saveAndGetScores(const unsigned int score, std::array<ScoreEntry, 5>& scores)
  {
    // @YOLO: can definitely be improved in many ways
    const auto t = std::chrono::system_clock::now(); // boring bit
    const auto timeT = std::chrono::system_clock::to_time_t(t); // boring bit
    const auto localTime = *std::localtime(&timeT); // boring bit
    std::stringstream ss;	// cut this out
    ss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S"); // cut this out
    const auto timeStr = ss.str(); // cut this out
    std::ifstream fileread("scoreboard.dat", std::ios::binary);
    if(fileread.is_open()) {
      // if the file already exists, load scores
      for(unsigned int i{ 0 }; i < scores.size(); ++i) {
	fileread.read(reinterpret_cast<char*>(scores[i].m_datetimebuff.data()), sizeof(scores[i].m_datetimebuff));
	fileread.read(reinterpret_cast<char*>(&scores[i].m_score), sizeof(scores[i].m_score));
	scores[i].m_currentScore = false;
      }
    }
    bool topfive{ false };
    auto it = std::find_if(scores.begin(), scores.end(), [score](const ScoreEntry& a){ return score > a.m_score; });
    if(it != scores.end()) {
      topfive = true;
    }
    // since the file is sorted at all times, you can just replace the last element and then sort
    if(topfive) {
      scores[4].m_score = score;
      scores[4].m_currentScore = true;
      std::copy(timeStr.begin(), timeStr.end(), scores[4].m_datetimebuff.begin());
      std::sort(scores.begin(), scores.end(), [](const ScoreEntry& a, const ScoreEntry& b) {
	return a.m_score >= b.m_score;
      });
    }
    // write back
    std::ofstream filewrite("scoreboard.dat", std::ios::binary);
    if(filewrite.is_open()) {
      for(const auto& e : scores) {
	filewrite.write(reinterpret_cast<const char*>(e.m_datetimebuff.data()), sizeof(e.m_datetimebuff));
	filewrite.write(reinterpret_cast<const char*>(&e.m_score), sizeof(e.m_score));
      }
    } else {
      std::clog << __FUNCTION__ << ": couldn't save scoreboard.\n";
    }
    return topfive;
  }

};

// !!! One translation unit to speed up compilation !!!
#include "invaders_resources.cpp"
#include "invaders_input.cpp"
#include "invaders_player.cpp"
#include "invaders_enemy.cpp"
#include "invaders_missile.cpp"
#include "invaders_explosion.cpp"
#include "invaders_grid.cpp"
#include "invaders_sim.cpp"
#include "invaders_renderer.cpp"
#include "invaders_level.cpp"
#include "invaders_text.cpp"
#include "invaders_menu.cpp"

// this depends on the underlying platform, in this case, x11, that's why
// this function it's here and not within the InputManager
static Input::Key map_x11_key_to_game(const KeySym& key)
{
  switch(key) {
  case XK_a:      return Input::Key::KEY_A;
  case XK_d:      return Input::Key::KEY_D;
  case XK_q:      return Input::Key::KEY_Q;
  case XK_Escape: return Input::Key::KEY_ESCAPE;
  case XK_space:  return Input::Key::KEY_SPACE;
  case XK_Left:   return Input::Key::KEY_LEFT;
  case XK_Right:  return Input::Key::KEY_RIGHT;
  case XK_Return: return Input::Key::KEY_ENTER;
  case XK_Up:     return Input::Key::KEY_UP;
  case XK_Down:   return Input::Key::KEY_DOWN;
  case XK_w:      return Input::Key::KEY_W;
  case XK_s:      return Input::Key::KEY_S;
  case XK_p:      return Input::Key::KEY_P;
  default:        return Input::Key::KEY_UNKNOWN;
  }
}

int main()
{
  // if we leave sync enabled it will slow down iostream a lot, don't need sync
  // between C and C++ streams anyways
  std::ios::sync_with_stdio(false);
  // first, init OpenGL's function pointers, this is probably specific for each
  // platform, so that's why it's in here; for another platform, say win32, the
  // idea is to have a win32_invaders.cpp and init OpenGL's function pointers in
  // a similar fashion
  Game::initOpenGLfptrs();
  // @NOTE: you need to initialise SDL here bc otherwise you cannot get audio devices
  // names b4 calling Mix_OpenAudioDevice! honestly, you could, but then you'd need to
  // have more deps just to do that (alsa or pulseaudio), not worth imho
  if(SDL_Init(SDL_INIT_AUDIO) < 0) {
    std::cerr << __FUNCTION__ << ": couldn't initialise SDL: " << SDL_GetError() << '\n';
    return EXIT_FAILURE;
  }
  // ---------------------------------------------
  // X11 specific code for init a window with OpenGL.
  // Don't know why X11, maybe should've used Wayland instead, well, cba
  // ---------------------------------------------
  // get a matching fb config
  static int attrs[] = {
    GLX_X_RENDERABLE    , True,
    GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
    GLX_RENDER_TYPE     , GLX_RGBA_BIT,
    GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
    GLX_RED_SIZE        , 8,
    GLX_GREEN_SIZE      , 8,
    GLX_BLUE_SIZE       , 8,
    GLX_ALPHA_SIZE      , 8,
    GLX_DEPTH_SIZE      , 24,
    GLX_STENCIL_SIZE    , 8,
    GLX_DOUBLEBUFFER    , True,
    GLX_SAMPLE_BUFFERS  , 1,
    GLX_SAMPLES         , 4,
    None
  };
  XSetWindowAttributes setWindowAttrs;
  XWindowAttributes gwa;
  Display* display = XOpenDisplay(NULL);
  if(!display) {
    std::cerr << "Couldn't connect to X server\n";
    std::exit(EXIT_FAILURE);
  }
  // grab best fb
  int best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;
  int fbcount;
  GLXFBConfig* fbc = glXChooseFBConfig(display, DefaultScreen(display), attrs, &fbcount);
  for(int i = 0; i < fbcount; ++i) {
    XVisualInfo* vi = glXGetVisualFromFBConfig(display, fbc[i]);
    if(vi) {
      int samp_buf, samples;
      glXGetFBConfigAttrib(display, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf );
      glXGetFBConfigAttrib(display, fbc[i], GLX_SAMPLES       , &samples  );
      if(best_fbc < 0 || (samp_buf && samples > best_num_samp)) {
	best_fbc = i;
	best_num_samp = samples;
      }
      if(worst_fbc < 0 || !samp_buf || samples < worst_num_samp) {
	worst_fbc = i;
	worst_num_samp = samples;
      }
    }
    XFree(vi);
  }
  GLXFBConfig bestFbc = fbc[best_fbc];
  // be sure to free the FBConfig list allocated by glXChooseFBConfig()
  XFree(fbc);
  // this root window is needed by X11, but useless for us; the interesting window is the next one
  Window root = DefaultRootWindow(display);
  XVisualInfo* visualInfo = glXGetVisualFromFBConfig(display, bestFbc);
  if(!visualInfo) {
    std::cerr << "Couldn't find appropiate visual info\n";
    std::exit(EXIT_FAILURE);
  }
  Colormap cmap = XCreateColormap(display, root, visualInfo->visual, AllocNone);
  // listening for key presses, releases and exposures
  setWindowAttrs.colormap = cmap;
  setWindowAttrs.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask;
  Window window = XCreateWindow(display, root, 0, 0,
                                WINDOW_WIDTH, WINDOW_HEIGHT, 0, visualInfo->depth,
                                InputOutput, visualInfo->visual, CWColormap | CWEventMask, &setWindowAttrs);
  XFree(visualInfo);
  XStoreName(display, window, WINDOW_NAME_GNU_LINUX);
  XMapWindow(display, window);
  // disable resizing
  XSizeHints hints;
  hints.flags = PMinSize | PMaxSize;
  hints.min_width = hints.max_width = WINDOW_WIDTH;
  hints.min_height = hints.max_height = WINDOW_HEIGHT;
  XSetWMNormalHints(display, window, &hints);
  // closing window gracefully when user clicks on the X btn
  Atom wmDeleteMessage = XInternAtom(display, "WM_DELETE_WINDOW", False);
  XSetWMProtocols(display, window, &wmDeleteMessage, 1);
  // pretty important, setup opengl context for renderdoc
  glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
  glXCreateContextAttribsARB =
    (glXCreateContextAttribsARBProc) glXGetProcAddressARB((const GLubyte *) "glXCreateContextAttribsARB");
  int contextAttribs[] = {
    GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
    GLX_CONTEXT_MINOR_VERSION_ARB, 3,
    GLX_CONTEXT_FLAGS_ARB        , GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
    None
  };
  // !!!renderdoc!!!
  GLXContext glContext = glXCreateContextAttribsARB(display, bestFbc, 0, True, contextAttribs);
  XSync(display, False);
  glXMakeCurrent(display, window, glContext);
  // enable VSync
  glXSwapIntervalEXT(display, window, 1);
  XEvent xev;
  // once OpenGL and X11 are ready to fight, game init takes place.
  // init all resource managers game has. If there's any error initialising them,
  // the game will straight up close with a diagnostic msg
  Ev::EventManager eventManager;
  Res::ResourceManager resourceManager(eventManager);
  Input::InputManager inputManager;
  Game::ExplosionManager explosionManager(resourceManager, eventManager);
  Game::GridManager gridManager(WINDOW_WIDTH, WINDOW_HEIGHT, eventManager);
  Game::MissileManager missileManager(resourceManager, gridManager, eventManager);
  Game::PlayerManager playerManager(resourceManager, inputManager, missileManager, gridManager, eventManager);
  Game::EnemyManager enemyManager(resourceManager, missileManager, gridManager, eventManager);
  Game::LevelManager levelManager(enemyManager);
  Renderer::TextRenderer textRenderer("./res/fonts/FreeMonoBold.ttf",
                                      28,
                                      resourceManager);
  Game::MenuManager menuManager(eventManager, inputManager);
  Renderer::RendererManager renderManager(resourceManager, textRenderer, menuManager);
  Sim::SimulationManager simulationManager(resourceManager,
                                           inputManager,
                                           playerManager,
                                           enemyManager,
                                           missileManager,
                                           explosionManager,
                                           gridManager,
                                           renderManager,
                                           menuManager,
                                           eventManager,
                                           levelManager,
                                           WINDOW_WIDTH,
                                           WINDOW_HEIGHT);
  // run the game
  auto lastFrame = 0.0f;
  while(!simulationManager.shouldEnd()) {
    // computing the delta time like this is a good idea, but remember that this delta
    // time corresponds to the previous frame, not the current one, so that's problematic
    // if this frame runs slower than the previous one. To solve this, if this frame ends
    // faster than expected, you put it to sleep for the remaining duration, effectively
    // making all frames render at the same time, howver not doing it for now
    auto currentFrame = Game::time();
    auto delta = currentFrame - lastFrame;
    lastFrame = currentFrame;
    glViewport(0, 0, gwa.width, gwa.height);
    inputManager.beginFrame();
    // polling for x11 window events
    while(XPending(display)) {
      XNextEvent(display, &xev);
      if(xev.type == KeyPress || xev.type == KeyRelease) {
        const auto is_pressed = xev.type == KeyPress;
        const auto x_key = XLookupKeysym(&xev.xkey, 0);
        const auto key = map_x11_key_to_game(x_key);
        inputManager.updateKey(key, is_pressed);
      } else if(xev.type == ClientMessage) {
        if(static_cast<Atom>(xev.xclient.data.l[0]) == wmDeleteMessage) {
          simulationManager.setShouldEnd(true);
        }
      }
    }
    // the simulation manager will update the world and then it'll call the renderer
    simulationManager.update(delta);
    XGetWindowAttributes(display, window, &gwa);
    glXSwapBuffers(display, window);
  }
  glXMakeCurrent(display, None, nullptr);
  glXDestroyContext(display, glContext);
  XDestroyWindow(display, window);
  XCloseDisplay(display);
  return EXIT_SUCCESS;
}
