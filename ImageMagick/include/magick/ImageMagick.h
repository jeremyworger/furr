/*
  ImageMagick Structure Members.
*/
#ifndef _IMAGEMAGICK_H
#define _IMAGEMAGICK_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#if !defined(QuantumDepth)
#define QuantumDepth  16
#define QuantumLeap
#endif

#if (QuantumDepth == 8)
#define MagickEpsilon  1.0e-6
#define MaxColormapSize  256
#define MaxMap  MaxRGB
#define MaxRGB  255
#define ScaleCharToQuantum(value)  ((Quantum) (value))
#define ScaleLongToQuantum(value)  ((Quantum) ((value)/16843009UL))
#define ScaleMapToQuantum(value) ((Quantum) (value))
#define ScaleQuantum(quantum)  ((unsigned long) (quantum))
#define ScaleQuantumToChar(quantum)  ((unsigned char) (quantum))
#define ScaleQuantumToIndex(value)   ((unsigned char) (value))
#define ScaleQuantumToLong(quantum)  ((unsigned long) (16843009UL*(quantum)))
#define ScaleQuantumToMap(quantum)  ((unsigned char) (quantum))
#define ScaleQuantumToShort(quantum)  ((unsigned short) (257*(quantum)))
#define ScaleShortToQuantum(value)  ((Quantum) ((value)/257))
#define ScaleToQuantum(value)  ((unsigned long) (value))

typedef float MagickRealType;
typedef unsigned char Quantum;
#elif (QuantumDepth == 16)
#define MagickEpsilon  1.0e-10
#define MaxColormapSize  65536
#define MaxMap  MaxRGB
#define MaxRGB  65535
#define ScaleCharToQuantum(value)  ((Quantum) (257*(value)))
#define ScaleLongToQuantum(value)  ((Quantum) ((value)/65537UL))
#define ScaleMapToQuantum(value)  ((Quantum) (value))
#define ScaleQuantum(quantum)  ((unsigned long) ((quantum)/257))
#define ScaleQuantumToChar(quantum)  ((unsigned char) ((quantum)/257))
#define ScaleQuantumToIndex(value)   ((unsigned short) (value))
#define ScaleQuantumToLong(quantum)  ((unsigned long) (65537UL*(quantum)))
#define ScaleQuantumToMap(quantum)  ((unsigned short) (quantum))
#define ScaleQuantumToShort(quantum)  ((unsigned short) (quantum))
#define ScaleShortToQuantum(value)  ((Quantum) (value))
#define ScaleToQuantum(value)  ((unsigned long) (257*(value)))

typedef double MagickRealType;
typedef unsigned short Quantum;
#elif (QuantumDepth == 32)
#define MagickEpsilon  1.0e-10
#define MaxColormapSize  65536
#define MaxMap  65535
#define MaxRGB  4294967295UL
#define ScaleCharToQuantum(value)  ((Quantum) (16843009UL*value))
#define ScaleLongToQuantum(value)  ((Quantum) (value))
#define ScaleMapToQuantum(value)  ((Quantum) (65537UL*value))
#define ScaleQuantum(quantum)  ((unsigned long) ((quantum)/16843009UL))
#define ScaleQuantumToChar(quantum)  ((unsigned char) ((quantum)/16843009UL))
#define ScaleQuantumToIndex(value)   ((unsigned short) ((value)/65537UL))
#define ScaleQuantumToLong(quantum)  ((unsigned long) (quantum))
#define ScaleQuantumToMap(quantum)  ((unsigned short) ((quantum)/65537UL))
#define ScaleQuantumToShort(quantum)  ((unsigned short) ((quantum)/65537UL))
#define ScaleShortToQuantum(value)  ((Quantum) (65537UL*(value)))
#define ScaleToQuantum(value)  ((unsigned long) (16843009UL*(value)))

#if defined(HAVE_LONG_DOUBLE)
typedef long double MagickRealType;
#else
typedef double MagickRealType;
#endif
typedef unsigned int Quantum;
#else
# error "Specified value of QuantumDepth is not supported"
#endif
#define AbsoluteValue(x)  ((x) < 0 ? -(x) : (x))
#define DegreesToRadians(x) (MagickPI*(x)/180.0)
#define IsGray(color)  ((MagickBooleanType) ( \
  (((color).red == (color).green) && ((color).green == (color).blue))))
#define MagickPI 3.14159265358979323846264338327950288419716939937510
#define MagickSQ2PI 2.50662827463100024161235523934010416269302368164062
#define MagickFalse  ((MagickBooleanType) 0x00)
#define MagickTrue  ((MagickBooleanType) 0x01)
#define Max(x,y)  (((x) > (y)) ? (x) : (y))
#define Min(x,y)  (((x) < (y)) ? (x) : (y))
#define QuantumScale  ((MagickRealType) 1.0/(MagickRealType) MaxRGB)
#define QuantumTick(i,span) ((((i) & 0xff) == 0) || \
  ((MagickOffsetType) i == ((MagickOffsetType) (span)-1)))
#define RadiansToDegrees(x) (180.0*(x)/MagickPI)
#define RoundToQuantum(value) ((Quantum) ((value) < 0.0 ? 0.0 : \
  ((value) > (MagickRealType) MaxRGB) ? (MagickRealType) MaxRGB : (value)+0.5))
#define RoundToMap(value) ((unsigned long) (value < 0.0 ? 0.0 : \
  ((value) > (MagickRealType) MaxMap) ? (MagickRealType) MaxMap : (value)+0.5))
#define ScaleXToQuantum(x,scale) ((Quantum) \
  (((MagickRealType) MaxRGB*(x))/(scale)+0.5))
#define Swap(x,y) ((x)^=(y), (y)^=(x), (x)^=(y))

/*
  3D effects.
*/
#define AccentuateModulate  ScaleCharToQuantum(80)
#define HighlightModulate  ScaleCharToQuantum(125)
#define ShadowModulate  ScaleCharToQuantum(135)
#define DepthModulate  ScaleCharToQuantum(185)
#define TroughModulate  ScaleCharToQuantum(110)

/*
  Typedef declarations.
*/

typedef unsigned int MagickBooleanType;
typedef unsigned int MagickStatusType;
#if !defined(__WINDOWS__)
#if (SIZEOF_LONG_LONG == 8)
typedef long long MagickOffsetType;
typedef unsigned long long MagickSizeType;
#else
typedef long MagickOffsetType;
typedef unsigned long MagickSizeType;
#endif
#else
typedef __int64 MagickOffsetType;
typedef unsigned __int64 MagickSizeType;
#endif

#if defined(macintosh)
#define ExceptionInfo MagickExceptionInfo
#endif

typedef enum
{
  UndefinedChannel,
  RedChannel = 0x0001,
  CyanChannel = 0x0001,
  GreenChannel = 0x0002,
  MagentaChannel = 0x0002,
  BlueChannel = 0x0004,
  YellowChannel = 0x0004,
  AlphaChannel = 0x0008,
  OpacityChannel = 0x0008,
  MatteChannel = 0x0008,  /* deprecated */
  BlackChannel = 0x0020,
  IndexChannel = 0x0020,
  AllChannels = 0x7fffffff
} ChannelType;

typedef enum
{
  UndefinedClass,
  DirectClass,
  PseudoClass
} ClassType;

typedef enum
{
  UndefinedColorspace,
  RGBColorspace,
  GRAYColorspace,
  TransparentColorspace,
  OHTAColorspace,
  LABColorspace,
  XYZColorspace,
  YCbCrColorspace,
  YCCColorspace,
  YIQColorspace,
  YPbPrColorspace,
  YUVColorspace,
  CMYKColorspace,
  sRGBColorspace,
  HSBColorspace,
  HSLColorspace,
  HWBColorspace
} ColorspaceType;

typedef enum
{
  UndefinedCompositeOp,
  NoCompositeOp,
  AddCompositeOp,
  AtopCompositeOp,
  BlendCompositeOp,
  BumpmapCompositeOp,
  ClearCompositeOp,
  ColorBurnCompositeOp,
  ColorDodgeCompositeOp,
  ColorizeCompositeOp,
  CopyBlackCompositeOp,
  CopyBlueCompositeOp,
  CopyCompositeOp,
  CopyCyanCompositeOp,
  CopyGreenCompositeOp,
  CopyMagentaCompositeOp,
  CopyOpacityCompositeOp,
  CopyRedCompositeOp,
  CopyYellowCompositeOp,
  DarkenCompositeOp,
  DstAtopCompositeOp,
  DstCompositeOp,
  DstInCompositeOp,
  DstOutCompositeOp,
  DstOverCompositeOp,
  DifferenceCompositeOp,
  DisplaceCompositeOp,
  DissolveCompositeOp,
  ExclusionCompositeOp,
  HardLightCompositeOp,
  HueCompositeOp,
  InCompositeOp,
  LightenCompositeOp,
  LuminizeCompositeOp,
  MinusCompositeOp,
  ModulateCompositeOp,
  MultiplyCompositeOp,
  OutCompositeOp,
  OverCompositeOp,
  OverlayCompositeOp,
  PlusCompositeOp,
  ReplaceCompositeOp,
  SaturateCompositeOp,
  ScreenCompositeOp,
  SoftLightCompositeOp,
  SrcAtopCompositeOp,
  SrcCompositeOp,
  SrcInCompositeOp,
  SrcOutCompositeOp,
  SrcOverCompositeOp,
  SubtractCompositeOp,
  ThresholdCompositeOp,
  XorCompositeOp
} CompositeOperator;

typedef enum
{
  UndefinedCompression,
  NoCompression,
  BZipCompression,
  FaxCompression,
  Group4Compression,
  JPEGCompression,
  LosslessJPEGCompression,
  LZWCompression,
  RLECompression,
  ZipCompression
} CompressionType;

typedef enum
{
  UnrecognizedDispose,
  UndefinedDispose = 0,
  NoneDispose = 1,
  BackgroundDispose = 2,
  PreviousDispose = 3
} DisposeType;

typedef enum
{
  UndefinedEndian,
  LSBEndian,
  MSBEndian
} EndianType;

typedef enum
{
  UndefinedException,
  WarningException = 300,
  ResourceLimitWarning = 300,
  TypeWarning = 305,
  OptionWarning = 310,
  DelegateWarning = 315,
  MissingDelegateWarning = 320,
  CorruptImageWarning = 325,
  FileOpenWarning = 330,
  BlobWarning = 335,
  StreamWarning = 340,
  CacheWarning = 345,
  CoderWarning = 350,
  ModuleWarning = 355,
  DrawWarning = 360,
  ImageWarning = 365,
  WandWarning = 370,
  XServerWarning = 380,
  MonitorWarning = 385,
  RegistryWarning = 390,
  ConfigureWarning = 395,
  ErrorException = 400,
  ResourceLimitError = 400,
  TypeError = 405,
  OptionError = 410,
  DelegateError = 415,
  MissingDelegateError = 420,
  CorruptImageError = 425,
  FileOpenError = 430,
  BlobError = 435,
  StreamError = 440,
  CacheError = 445,
  CoderError = 450,
  ModuleError = 455,
  DrawError = 460,
  ImageError = 465,
  WandError = 470,
  XServerError = 480,
  MonitorError = 485,
  RegistryError = 490,
  ConfigureError = 495,
  FatalErrorException = 700,
  ResourceLimitFatalError = 700,
  TypeFatalError = 705,
  OptionFatalError = 710,
  DelegateFatalError = 715,
  MissingDelegateFatalError = 720,
  CorruptImageFatalError = 725,
  FileOpenFatalError = 730,
  BlobFatalError = 735,
  StreamFatalError = 740,
  CacheFatalError = 745,
  CoderFatalError = 750,
  ModuleFatalError = 755,
  DrawFatalError = 760,
  ImageFatalError = 765,
  WandFatalError = 770,
  XServerFatalError = 780,
  MonitorFatalError = 785,
  RegistryFatalError = 790,
  ConfigureFatalError = 795
} ExceptionType;

typedef enum
{
  UndefinedFilter,
  PointFilter,
  BoxFilter,
  TriangleFilter,
  HermiteFilter,
  HanningFilter,
  HammingFilter,
  BlackmanFilter,
  GaussianFilter,
  QuadraticFilter,
  CubicFilter,
  CatromFilter,
  MitchellFilter,
  LanczosFilter,
  BesselFilter,
  SincFilter
} FilterTypes;

#ifdef ForgetGravity
#undef ForgetGravity
#undef NorthWestGravity
#undef NorthGravity
#undef NorthEastGravity
#undef WestGravity
#undef CenterGravity
#undef EastGravity
#undef SouthWestGravity
#undef SouthGravity
#undef SouthEastGravity
#undef StaticGravity
#endif

typedef enum
{
  UndefinedGravity,
  ForgetGravity = 0,
  NorthWestGravity = 1,
  NorthGravity = 2,
  NorthEastGravity = 3,
  WestGravity = 4,
  CenterGravity = 5,
  EastGravity = 6,
  SouthWestGravity = 7,
  SouthGravity = 8,
  SouthEastGravity = 9,
  StaticGravity = 10
} GravityType;

typedef enum
{
  UndefinedType,
  BilevelType,
  GrayscaleType,
  GrayscaleMatteType,
  PaletteType,
  PaletteMatteType,
  TrueColorType,
  TrueColorMatteType,
  ColorSeparationType,
  ColorSeparationMatteType,
  OptimizeType
} ImageType;

typedef enum
{
  UndefinedInterlace,
  NoInterlace,
  LineInterlace,
  PlaneInterlace,
  PartitionInterlace
} InterlaceType;

typedef enum
{
  UndefinedOrientation,
  TopLeftOrientation,
  TopRightOrientation,
  BottomRightOrientation,
  BottomLeftOrientation,
  LeftTopOrientation,
  RightTopOrientation,
  RightBottomOrientation,
  LeftBottomOrientation
} OrientationType;

typedef enum
{
  UndefinedPreview,
  RotatePreview,
  ShearPreview,
  RollPreview,
  HuePreview,
  SaturationPreview,
  BrightnessPreview,
  GammaPreview,
  SpiffPreview,
  DullPreview,
  GrayscalePreview,
  QuantizePreview,
  DespecklePreview,
  ReduceNoisePreview,
  AddNoisePreview,
  SharpenPreview,
  BlurPreview,
  ThresholdPreview,
  EdgeDetectPreview,
  SpreadPreview,
  SolarizePreview,
  ShadePreview,
  RaisePreview,
  SegmentPreview,
  SwirlPreview,
  ImplodePreview,
  WavePreview,
  OilPaintPreview,
  CharcoalDrawingPreview,
  JPEGPreview
} PreviewType;

typedef enum
{
  UndefinedIntent,
  SaturationIntent,
  PerceptualIntent,
  AbsoluteIntent,
  RelativeIntent
} RenderingIntent;

typedef enum
{
  UndefinedResolution,
  PixelsPerInchResolution,
  PixelsPerCentimeterResolution
} ResolutionType;

typedef enum
{
  UndefinedTimerState,
  StoppedTimerState,
  RunningTimerState
} TimerState;

typedef struct _AffineMatrix
{
  double
    sx,
    rx,
    ry,
    sy,
    tx,
    ty;
} AffineMatrix;

typedef Quantum IndexPacket;

typedef struct _PixelPacket
{
#if defined(WORDS_BIGENDIAN)
  Quantum
    red,
    green,
    blue,
    opacity;
#else
#if defined(macintosh)
  Quantum
    opacity,
    red,
    green,
    blue;
#else
  Quantum
    blue,
    green,
    red,
    opacity;
#endif
#endif
} PixelPacket;

typedef struct _ColorPacket
{
  PixelPacket
    pixel;

  IndexPacket
    index;

  unsigned long
    count;
} ColorPacket;

typedef struct _ErrorInfo
{
  double
    mean_error_per_pixel,
    normalized_mean_error,
    normalized_maximum_error;
} ErrorInfo;

typedef struct _PrimaryInfo
{
  double
    x,
    y,
    z;
} PrimaryInfo;

typedef struct _ProfileInfo
{
  char
    *name;

  size_t
    length;

  unsigned char
    *info;

  unsigned long
    signature;
} ProfileInfo;

typedef struct _RectangleInfo
{
  unsigned long
    width,
    height;

  long
    x,
    y;
} RectangleInfo;

typedef struct _Timer
{
  double
    start,
    stop,
    total;
} Timer;

typedef struct _SegmentInfo
{
  double
    x1,
    y1,
    x2,
    y2;
} SegmentInfo;

typedef struct _TimerInfo
{
  Timer
    user,
    elapsed;

  TimerState
    state;

  unsigned long
    signature;
} TimerInfo;

typedef struct _ChromaticityInfo
{
  PrimaryInfo
    red_primary,
    green_primary,
    blue_primary,
    white_point;
} ChromaticityInfo;

typedef struct _ExceptionInfo
{
  ExceptionType
    severity;

  int
    error_number;

  char
    *reason,
    *description;

  unsigned long
    signature;
} ExceptionInfo;

typedef struct _Ascii85Info _Ascii85Info_;

typedef struct _BlobInfo _BlobInfo_;

typedef struct _HashmapInfo _HashmapInfo_;

typedef struct _ImageAttribute _ImageAttribute_;

typedef struct _SemaphoreInfo _SemaphoreInfo_;

typedef struct _Image
{
  ClassType
    storage_class;

  ColorspaceType
    colorspace;

  CompressionType
    compression;

  OrientationType
    orientation;

  MagickBooleanType
    taint,
    matte;

  unsigned long
    columns,
    rows;

  unsigned long
    depth,
    colors;

  PixelPacket
    *colormap,
    background_color,
    border_color,
    matte_color;

  double
    gamma;

  ChromaticityInfo
    chromaticity;

  RenderingIntent
    rendering_intent;

  _HashmapInfo_
    *profiles;

  ResolutionType
    units;

  char
    *montage,
    *directory,
    *geometry;

  long
    offset;

  double
    x_resolution,
    y_resolution;

  RectangleInfo
    page,
    extract_info,
    tile_info;  /* deprectated */

  double
    blur,
    fuzz;

  FilterTypes
    filter;

  InterlaceType
    interlace;

  EndianType
    endian;

  GravityType
    gravity;

  CompositeOperator
    compose;

  DisposeType
    dispose;

  struct _Image
    *clip_mask;

  unsigned long
    scene,
    delay,
    iterations,
    total_colors;

  long
    start_loop;

  ErrorInfo
    error;

  TimerInfo
    timer;

  void
    *client_data;

  void
    *cache;

  _ImageAttribute_
    *attributes;

  _Ascii85Info_
    *ascii85;

  _BlobInfo_
    *blob;

  char
    filename[MaxTextExtent],
    magick_filename[MaxTextExtent],
    magick[MaxTextExtent];

  unsigned long
    magick_columns,
    magick_rows;

  ExceptionInfo
    exception;

  MagickBooleanType
    debug;

  long
    reference_count;

  _SemaphoreInfo_
    *semaphore;

  ProfileInfo
    color_profile,
    iptc_profile,
    *generic_profile;

  unsigned long
    generic_profiles;  /* this & ProfileInfo is deprecated */

  unsigned long
    signature;

  struct _Image
    *previous,
    *list,
    *next;
} Image;

typedef MagickBooleanType
  (*StreamHandler)(const Image *,const void *,const size_t);

typedef struct _ImageInfo
{
  CompressionType
    compression;

  OrientationType
    orientation;

  MagickBooleanType
    temporary,
    adjoin,
    affirm,
    antialias;

  char
    *size,
    *extract,
    *page,
    *scenes;

  unsigned long
    scene,
    number_scenes,
    depth;

  InterlaceType
    interlace;

  EndianType
    endian;

  ResolutionType
    units;

  unsigned long
    quality;

  char
    *sampling_factor,
    *server_name,
    *font,
    *texture,
    *density;

  double
    pointsize,
    fuzz;

  PixelPacket
    background_color,
    border_color,
    matte_color;

  MagickBooleanType
    dither,
    monochrome;

  unsigned long
    colors;

  ColorspaceType
    colorspace;

  ImageType
    type;

  PreviewType
    preview_type;

  long
    group;

  MagickBooleanType
    ping,
    verbose;

  char
    *view,
    *authenticate;

  Image
    *attributes;

  _HashmapInfo_
    *options;

  void
    *client_data;

  void
    *cache;

  StreamHandler
    stream;

  FILE
    *file;

  void
    *blob;

  size_t
    length;

  char
    magick[MaxTextExtent],
    unique[MaxTextExtent],
    zero[MaxTextExtent],
    filename[MaxTextExtent];

  MagickBooleanType
    debug;

  /*
    Deprecated.
  */
  char
    *tile;

  unsigned long
    subimage,
    subrange;

  PixelPacket
    pen;

  unsigned long
    signature;
} ImageInfo;

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
